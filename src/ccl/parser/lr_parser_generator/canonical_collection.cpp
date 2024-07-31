#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    auto LrParserGenerator::gotoFunction(const std::list<LrItem> &items, Symbol symbol) const
        -> std::list<LrItem>
    {
        auto moved = std::list<LrItem>{};

        for (const auto &item : items) {
            if (item.isDotInTheEnd()) {
                continue;
            }

            if (item.at(item.getDotLocation()) == symbol) {
                moved.emplace_back(
                    std::addressof(item.getRule()), item.getDotLocation() + 1,
                    item.getProductionType(), item.getLookAhead());
            }
        }

        return computeClosure(std::move(moved));
    }

    auto LrParserGenerator::doCanonicalCollectionConstructionIterationOnItem(
        Id &closure_id, const CanonicalCollection &cc, const LrItem &item) -> bool
    {
        auto has_changes = false;

        for (std::size_t i = item.getDotLocation(); i != item.size(); ++i) {
            auto goto_result = gotoFunction(cc.items, item.at(i));
            auto temp_cc_id = closure_id;

            auto temp_cc = CanonicalCollection{
                .items = goto_result,
                .id = temp_cc_id,
            };

            if (auto cc_it = std::ranges::find(canonicalCollection, temp_cc);
                cc_it != canonicalCollection.end()) {
                temp_cc_id = cc_it->id;
            } else {
                ++closure_id;
                has_changes = true;
                canonicalCollection.emplace_back(std::move(temp_cc));
            }

            transitions.try_emplace({cc.id, item.at(i)}, temp_cc_id);
        }

        return has_changes;
    }

    auto LrParserGenerator::doCanonicalCollectionConstructionIteration(
        Id &closure_id, std::set<Id> &marked_collections) -> bool
    {
        auto has_new_sets = false;

        for (const auto &cc : canonicalCollection) {
            if (marked_collections.contains(cc.id)) {
                continue;
            }

            marked_collections.emplace(cc.id);

            for (const auto &item : cc.items) {
                has_new_sets =
                    doCanonicalCollectionConstructionIterationOnItem(closure_id, cc, item) ||
                    has_new_sets;
            }
        }

        return has_new_sets;
    }

    auto LrParserGenerator::constructCanonicalCollection(const LrItem &start_item) -> void
    {
        auto closure_id = Id{1};
        auto marked_collections = std::set<Id>{};

        canonicalCollection.emplace_back(
            CanonicalCollection{
                .items = computeClosure({start_item}),
                .id = 0,
            });

        CCL_REPEAT_WHILE(doCanonicalCollectionConstructionIteration(closure_id, marked_collections))
    }
}// namespace ccl::parser