#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    using namespace std::chrono_literals;

    auto LrParserGenerator::pushIntoPipe(GotoResult &value) -> void
    {
        while (!pipe.tryPush(value)) {
            // empty code
        }
    }

    auto LrParserGenerator::moveCollectionItemsOverSymbol(
        const CanonicalCollection &canonical_collection, Symbol symbol) -> std::vector<LrItem>
    {
        auto moved = std::vector<LrItem>{};
        moved.reserve(canonical_collection.items.size());

        for (const auto &item : canonical_collection.items) {
            if (item.isDotInTheEnd()) {
                continue;
            }

            const auto dot_index = item.getDotLocation();
            const auto symbol_at_dot = item.at(dot_index);

            if (symbol_at_dot == symbol) {
                moved.emplace_back(
                    item.getRulePtr(), item.getDotLocation() + 1, item.getProductionType(),
                    item.getLookAhead());
            }
        }

        return computeClosureOnItems(std::move(moved));
    }

    auto LrParserGenerator::moveCollectionItemsOverRemainingSymbols(
        const CanonicalCollection &canonical_collection) -> void
    {
        for (const auto &item : canonical_collection.items) {
            for (const auto symbol : item | std::views::drop(item.getDotLocation())) {
                auto value = GotoResult{
                    .items = moveCollectionItemsOverSymbol(canonical_collection, symbol),
                    .symbol = symbol,
                    .canonicalCollectionId = canonical_collection.id,
                };

                pushIntoPipe(value);
            }
        }
    }

    auto LrParserGenerator::pollCanonicalCollection() -> isl::Task<>
    {
        auto collection_end = canonicalCollection.end();
        auto max_polled_it_copy = lastPolledCanonicalCollection;

        for (auto it = max_polled_it_copy; it != collection_end; ++it) {
            max_polled_it_copy = it;
            moveCollectionItemsOverRemainingSymbols(*it);
        }

        lastPolledCanonicalCollection = max_polled_it_copy;
        pipe.producerDone();
        co_return;
    }

    auto LrParserGenerator::tryPopFromPipe(GotoResult &value) noexcept -> PipePopStatus
    {
        using enum ccl::parser::LrParserGenerator::PipePopStatus;

        if (!pipe.tryPop(value)) {
            if (pipe.isProducerDone()) {
                return PIPE_CLOSED;
            }

            return PIPE_EMPTY;
        }

        return SUCCEED;
    }

    auto LrParserGenerator::fillCanonicalCollection(isl::thread::IdGenerator &closure_id) -> bool
    {
        auto has_new_sets = false;
        auto result = GotoResult{};

        pipe.produceResume();
        auto task = runtime::async(pollCanonicalCollection());

        while (true) {
            auto pop_result = tryPopFromPipe(result);

            if (pop_result == PipePopStatus::PIPE_EMPTY) {
                std::this_thread::sleep_for(100us);
                continue;
            }

            if (pop_result == PipePopStatus::PIPE_CLOSED) {
                break;
            }

            auto &[goto_result, symbol, cc_id] = result;

            auto temp_cc_id = State{};
            auto temp_cc = CanonicalCollection{
                .items = std::move(goto_result),
                .id = 0,
            };

            auto cc_it = std::ranges::find(canonicalCollection, temp_cc);

            if (cc_it != canonicalCollection.end()) {
                temp_cc_id = cc_it->id;
            } else {
                has_new_sets = true;
                temp_cc_id = isl::as<SmallId>(closure_id.next());
                temp_cc.id = temp_cc_id;

                canonicalCollection.emplace_back(std::move(temp_cc));
                std::atomic_thread_fence(std::memory_order_release);
            }

            transitions.try_emplace({cc_id, symbol}, temp_cc_id);
        }

        task.await();
        return has_new_sets;
    }

    auto LrParserGenerator::constructCanonicalCollection(const LrItem &start_item) -> void
    {
        auto closure_id = isl::thread::IdGenerator{1};

        canonicalCollection.emplace_back(CanonicalCollection{
            .items = computeClosureOnItems({start_item}),
            .id = 0,
        });

        lastPolledCanonicalCollection = canonicalCollection.begin();
        CCL_REPEAT_WHILE(fillCanonicalCollection(closure_id))
    }
}// namespace ccl::parser