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
        const CanonicalCollection&canonical_collection, Symbol symbol) -> std::vector<LrItem> {
        auto moved = std::vector<LrItem>{};
        moved.reserve(canonical_collection.items.size());

        for (const auto &item : canonical_collection.items) {
            if (item.dottedRule.isDotInTheEnd()) {
                continue;
            }

            if (item.dottedRule.atDot() == symbol) {
                auto item_with_moved_dot = item;
                item_with_moved_dot.dottedRule.dotPosition += 1;
                moved.emplace_back(item_with_moved_dot);
            }
        }

        return computeClosureOnItems(std::move(moved));
    }

    auto LrParserGenerator::moveCollectionItemsOverRemainingSymbols(
        const CanonicalCollection &canonical_collection) -> void
    {
        for (const auto &item : canonical_collection.items) {
            for (const auto symbol : item | std::views::drop(item.dottedRule.dotPosition)) {
                auto value = GotoResult{
                    .items = moveCollectionItemsOverSymbol(canonical_collection, symbol),
                    .symbol = symbol,
                    .canonicalCollectionId = canonical_collection.id,
                };

                pushIntoPipe(value);
            }
        }
    }

    auto LrParserGenerator::generateCanonicalCollection() -> isl::Task<>
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
        using enum PipePopStatus;

        if (!pipe.tryPop(value)) {
            if (pipe.isProducerDone()) {
                return PIPE_CLOSED;
            }

            return PIPE_EMPTY;
        }

        return SUCCEED;
    }

    auto LrParserGenerator::fillCanonicalCollection(isl::thread::IdGenerator<SmallId> &closure_id)
        -> bool
    {
        using enum PipePopStatus;

        auto has_new_sets = false;
        auto result = GotoResult{};

        pipe.produceResume();
        auto task = runtime::async(generateCanonicalCollection());

        while (true) {
            auto pop_result = tryPopFromPipe(result);

            if (pop_result == PIPE_EMPTY) {
                std::this_thread::sleep_for(100us);
                continue;
            }

            if (pop_result == PIPE_CLOSED) {
                break;
            }

            auto &[goto_result, symbol, cc_id] = result;

            auto temp_cc = CanonicalCollection{
                .items = std::move(goto_result),
                .id = 0,
            };

            const auto entry = TableEntry{
                .state = cc_id,
                .symbol = symbol,
            };

            const auto canonical_collection_it = std::ranges::find(canonicalCollection, temp_cc);

            if (canonical_collection_it != canonicalCollection.end()) {
                transitions.try_emplace(entry, canonical_collection_it->id);
                continue;
            }

            has_new_sets = true;
            temp_cc.id = isl::as<SmallId>(closure_id.next());

            transitions.try_emplace(entry, temp_cc.id);
            canonicalCollection.emplace_back(std::move(temp_cc));
        }

        task.await();
        return has_new_sets;
    }

    auto LrParserGenerator::constructCanonicalCollection(const LrItem &start_item) -> void
    {
        auto closure_id = isl::thread::IdGenerator<SmallId>{1};

        canonicalCollection.emplace_back(
            CanonicalCollection{
                .items = computeClosureOnItems({start_item}),
                .id = 0,
            });

        lastPolledCanonicalCollection = canonicalCollection.begin();
        CCL_REPEAT_WHILE(fillCanonicalCollection(closure_id))
    }
}// namespace ccl::parser

auto fmt::formatter<ccl::parser::CanonicalCollectionPrintWrapper>::format(
    const ccl::parser::CanonicalCollectionPrintWrapper &collection_print_wrapper,
    format_context &ctx) -> format_context::iterator
{
    const auto &collection = collection_print_wrapper.canonicalCollection;

    return fmt::format_to(
        ctx.out(), "{}: {}", collection.id,
        std::views::transform(
            collection.items, [&collection_print_wrapper](const ccl::parser::LrItem &item) {
                return ccl::parser::LrItemPrintWrapper(
                    item, collection_print_wrapper.idToStringConversionFunction);
            }));
}
