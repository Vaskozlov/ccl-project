#include "ccl/parser/first_set.hpp"
#include "ccl/lexer/token.hpp"

namespace ccl::parser
{
    detail::FirstSetEvaluator::FirstSetEvaluator(
        Id epsilon_symbol, const isl::Set<Id> &grammar_symbols,
        const isl::Set<Id> &terminal_symbols,
        const isl::Map<Id, isl::Vector<isl::Vector<Id>>> &parser_rules)
      : FirstAndFollowSetsCommon::
            FirstAndFollowSetsCommon{grammar_symbols, terminal_symbols, parser_rules}
      , epsilon{epsilon_symbol}
    {
        initializeFirstSet();
        computeFirstSet();
    }

    auto detail::FirstSetEvaluator::initializeFirstSet() -> void
    {
        for (auto symbol : symbols) {
            if (!lexer::isUsedDefinedTokenOrEoF(symbol)) {
                continue;
            }

            if (isTerminal(symbol)) {
                firstSet.try_emplace(symbol, isl::Set<Id>{symbol});
            } else {
                firstSet.try_emplace(symbol);
            }
        }
    }

    auto detail::FirstSetEvaluator::computeFirstSet() -> void
    {
        applyFixedPointAlgorithmOnAllRules([this](Id key, const isl::Vector<Id> &rule) {
            return firstSetComputationIteration(key, rule);
        });
    }

    auto detail::FirstSetEvaluator::firstSetComputationIteration(
        Id key, const isl::Vector<Id> &rule) -> bool
    {
        const auto front_element = rule.front();
        const auto back_element = rule.back();
        const auto index_max = rule.size() - 1;
        auto index = isl::as<std::size_t>(0);

        auto rhs = firstSet.at(front_element);
        rhs.erase(epsilon);

        while (index != index_max && firstSet.at(rule.at(index)).contains(epsilon)) {
            ++index;

            auto elements_to_insert = firstSet.at(rule.at(index));
            elements_to_insert.erase(epsilon);

            insertRange(rhs, elements_to_insert);
        }

        if (index == index_max && firstSet[back_element].contains(epsilon)) {
            rhs.emplace(epsilon);
        }

        return insertRange(firstSet.at(key), rhs);
    }

    auto evaluateFirstSet(
        Id epsilon, const isl::Set<Id> &grammar_symbols, const isl::Set<Id> &terminals,
        const isl::Map<Id, isl::Vector<isl::Vector<Id>>> &rules) -> isl::Map<Id, isl::Set<Id>>
    {
        auto first_set = detail::FirstSetEvaluator(epsilon, grammar_symbols, terminals, rules);
        return std::move(first_set.getFirstSet());
    }
}// namespace ccl::parser