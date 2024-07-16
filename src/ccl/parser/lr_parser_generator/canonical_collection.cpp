#include "ccl/parser/lr/lr_parser_generator.hpp"

namespace ccl::parser
{
    auto LrParserGenerator::gotoFunction(const isl::Set<LrItem> &items, Symbol symbol) const
        -> isl::Set<LrItem>
    {
        auto moved = isl::Set<LrItem>{};

        for (const auto &item : items) {
            if (item.isDotInTheEnd()) {
                continue;
            }

            if (item.at(item.getDotLocation()) == symbol) {
                moved.emplace(
                    item.getRule(), item.getDotLocation() + 1, item.getProductionType(),
                    item.getLookAhead());
            }
        }

        return computeClosure(moved);
    }

    auto LrParserGenerator::doCanonicalCollectionConstructionIterationOnItem(
        Id &closure_id, const CanonicalCollection &cc, const LrItem &item,
        isl::Set<CanonicalCollection> &pending_collections) -> bool
    {
        auto has_new_sets = false;

        for (std::size_t i = item.getDotLocation(); i != item.length(); ++i) {
            auto goto_result = gotoFunction(cc.items, item.at(i));
            auto temp_cc_id = closure_id;

            auto temp_cc = CanonicalCollection{
                .items = goto_result,
                .id = temp_cc_id,
            };

            if (auto cc_it = canonicalCollection.find(temp_cc);
                cc_it != canonicalCollection.end()) {
                temp_cc_id = cc_it->id;
            } else if (auto pending_it = pending_collections.find(temp_cc);
                       pending_it != pending_collections.end()) {
                temp_cc_id = pending_it->id;
            } else {
                ++closure_id;
                has_new_sets = true;
                pending_collections.emplace(std::move(temp_cc));
            }

            transitions.try_emplace({cc.id, item.at(i)}, temp_cc_id);
        }

        return has_new_sets;
    }

    auto LrParserGenerator::doCanonicalCollectionConstructionIteration(
        Id &closure_id, isl::Set<Id> &marked_collections) -> bool
    {
        auto has_new_sets = false;
        auto pending_collections = isl::Set<CanonicalCollection>{};

        for (const auto &cc : canonicalCollection) {
            if (marked_collections.contains(cc.id)) {
                continue;
            }

            marked_collections.emplace(cc.id);

            for (const auto &item : cc.items) {
                has_new_sets = doCanonicalCollectionConstructionIterationOnItem(
                                   closure_id, cc, item, pending_collections) ||
                               has_new_sets;
            }
        }

        for (const auto &collection : pending_collections) {
            canonicalCollection.emplace(collection);
        }

        return has_new_sets;
    }

    auto LrParserGenerator::constructCanonicalCollection(const LrItem &start_item) -> void
    {
        auto closure_id = Id{1};
        auto has_new_sets = true;
        auto marked_collections = isl::Set<Id>{};

        canonicalCollection.emplace(CanonicalCollection{
            .items = computeClosure({start_item}),
            .id = 0,
        });

        CCL_REPEAT_WHILE(doCanonicalCollectionConstructionIteration(closure_id, marked_collections))
    }
}// namespace ccl::parser