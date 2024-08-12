#include "ccl/parser/detail/empty_moves_fixer.hpp"

namespace ccl::parser::detail
{
    EmptyMovesFixer::EmptyMovesFixer(GrammarStorage &grammar_storage)
      : possiblyEmptyRules({grammar_storage.getEpsilon()})
      , storage{grammar_storage}
    {}

    auto EmptyMovesFixer::apply() -> void
    {
        findEmptyRules();
        fixEmptyRules();
    }

    auto EmptyMovesFixer::fixRule(Symbol key, const Rule &rule, std::size_t current_symbol_index)
        -> void
    {
        for (auto i = current_symbol_index; i < rule.size(); ++i) {
            const auto symbol = rule.at(i);

            if (possiblyEmptyRules.contains(symbol)) {
                auto new_rule = Rule{rule};
                new_rule.erase(new_rule.cbegin() + static_cast<std::ptrdiff_t>(i));

                if (symbol == storage.getEpsilon()) {
                    pendingRulesToRemove.emplace_back(key, rule);
                }

                if (!new_rule.empty()) {
                    pendingRulesToAdd.emplace_back(key, new_rule);
                }

                fixRule(key, new_rule, i);
            }
        }
    }

    auto EmptyMovesFixer::findEmptyRules() -> void
    {
        auto has_changes = true;
        auto is_symbol_in_possible_empty = [this](Symbol symbol) {
            return possiblyEmptyRules.contains(symbol);
        };

        while (has_changes) {
            has_changes = false;

            for (auto &[key, rule] : storage.rulesIterator()) {
                if (std::ranges::all_of(rule, is_symbol_in_possible_empty)) {
                    auto [it, has_inserter] = possiblyEmptyRules.emplace(key);
                    has_changes = has_inserter;
                }
            }
        }
    }

    auto EmptyMovesFixer::fixEmptyRules() -> void
    {
        for (const auto &[key, rule] : storage.rulesIterator()) {
            fixRule(key, rule, 0);
        }

        for (auto &[key, rule] : pendingRulesToAdd) {
            storage.tryEmplace(key, std::move(rule));
        }

        for (const auto &[key, rule] : pendingRulesToRemove) {
            storage.eraseRule(key, rule);
        }
    }
}// namespace ccl::parser::detail
