#include "ccl/parser/first_set.hpp"
#include "ccl/lexer/token.hpp"

namespace ccl::parser
{
    detail::FirstSetEvaluator::FirstSetEvaluator(
        Production epsilon_symbol, const isl::Set<Production> &grammar_symbols,
        const isl::Set<Production> &terminal_symbols,
        const isl::Map<Production, isl::Vector<isl::Vector<Production>>> &parser_rules)
      : FirstAndFollowSetsCommon{grammar_symbols, terminal_symbols, parser_rules}
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
                firstSet.try_emplace(symbol, isl::Set<Production>{symbol});
            } else {
                firstSet.try_emplace(symbol);
            }
        }
    }

    auto detail::FirstSetEvaluator::computeFirstSet() -> void
    {
        applyFixedPointAlgorithmOnAllRules(
            [this](Production key, const isl::Vector<Production> &rule) {
                return firstSetComputationIteration(key, rule);
            });
    }

    auto detail::FirstSetEvaluator::firstSetComputationIteration(
        Production key, const isl::Vector<Production> &rule) -> bool
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
        Production epsilon, const isl::Set<Production> &grammar_symbols,
        const isl::Set<Production> &terminals,
        const isl::Map<Production, isl::Vector<isl::Vector<Production>>> &rules)
        -> isl::Map<Production, isl::Set<Production>>
    {
        auto first_set = detail::FirstSetEvaluator(epsilon, grammar_symbols, terminals, rules);
        return std::move(first_set.getFirstSet());
    }
}// namespace ccl::parser