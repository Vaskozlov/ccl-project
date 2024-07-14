#include "ccl/parser/lr_parser.hpp"

namespace ccl::parser
{
    auto LrParser::gotoFunction(const isl::Set<LrItem> &items, std::size_t product)
        -> isl::Set<LrItem>
    {
        auto moved = isl::Set<LrItem>{};

        for (const auto &item : items) {
            if (item.isDotInTheEnd()) {
                continue;
            }

            if (item.at(item.getDotLocation()) == product) {
                moved.emplace(
                    item.getRule(), item.getDotLocation() + 1, item.getProductionType(),
                    item.getLookAhead());
            }
        }

        return computeClosure(moved);
    }

    auto LrParser::doCanonicalCollectionConstructionIterationOnItem(
        Id &closure_id, const CanonicalCollection &cc, const LrItem &item,
        isl::Set<CanonicalCollection> &pending_collections) -> bool
    {
        auto has_new_sets = false;

        for (std::size_t i = item.getDotLocation(); i < item.length(); ++i) {
            auto goto_result = gotoFunction(cc.items, item.at(i));
            auto temp_cc_id = closure_id;
            auto temp_cc = CanonicalCollection{goto_result, temp_cc_id};

            if (auto cc_it = canonicalCollection.find(temp_cc);
                cc_it != canonicalCollection.end()) {
                temp_cc_id = cc_it->collectionId;
            } else if (auto pending_it = pending_collections.find(temp_cc);
                       pending_it != pending_collections.end()) {
                temp_cc_id = pending_it->collectionId;
            } else {
                ++closure_id;
                has_new_sets = true;
                pending_collections.emplace(std::move(temp_cc));
            }

            transitions.try_emplace({cc.collectionId, item.at(i)}, temp_cc_id);
        }

        return has_new_sets;
    }

    auto LrParser::doCanonicalCollectionConstructionIteration(
        Id &closure_id, isl::Set<Id> &marked_collections) -> bool
    {
        auto has_new_sets = false;
        auto pending_collections = isl::Set<CanonicalCollection>{};

        for (const auto &cc : canonicalCollection) {
            if (marked_collections.contains(cc.collectionId)) {
                continue;
            }

            marked_collections.emplace(cc.collectionId);

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

    auto LrParser::constructCanonicalCollection(const LrItem &start_item) -> void
    {
        auto closure_id = Id{};
        auto has_new_sets = true;
        auto marked_collections = isl::Set<Id>{};

        canonicalCollection.emplace(CanonicalCollection{computeClosure({start_item}), closure_id});
        ++closure_id;

        while (has_new_sets) {
            has_new_sets =
                doCanonicalCollectionConstructionIteration(closure_id, marked_collections);
        }
    }
}// namespace ccl::parser