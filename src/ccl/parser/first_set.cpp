#include "ccl/parser/first_set.hpp"
#include "ccl/lexer/token.hpp"

namespace ccl::parser
{
    detail::FirstSetEvaluator::FirstSetEvaluator(
        Symbol epsilon_symbol,
        const GrammarRulesStorage &parser_rules)
      : FirstAndFollowSetsCommon{parser_rules}
      , epsilonSymbol{epsilon_symbol}
    {
        initializeFirstSet();
        computeFirstSet();
    }

    auto detail::FirstSetEvaluator::initializeFirstSet() -> void
    {
        for (auto symbol : grammarRules.getGrammarSymbols()) {
            if (!lexer::isUsedDefinedTokenOrEoF(symbol)) {
                continue;
            }

            if (isTerminal(symbol)) {
                firstSet.try_emplace(symbol, std::set<Symbol>{symbol});
            } else {
                firstSet.try_emplace(symbol);
            }
        }
    }

    auto detail::FirstSetEvaluator::computeFirstSet() -> void
    {
        applyFixedPointAlgorithmOnAllRules([this](Symbol key, const Rule &rule) {
            return firstSetComputationIteration(key, rule);
        });
    }

    auto detail::FirstSetEvaluator::firstSetComputationIteration(Symbol key, const Rule &rule)
        -> bool
    {
        const auto front_element = rule.front();
        const auto back_element = rule.back();
        const auto index_maximum = rule.size() - 1;
        auto index = isl::as<std::size_t>(0);

        auto rhs = firstSet.at(front_element);
        rhs.erase(epsilonSymbol);

        while (index != index_maximum && firstSet.at(rule.at(index)).contains(epsilonSymbol)) {
            ++index;

            auto elements_to_insert = firstSet.at(rule.at(index));
            elements_to_insert.erase(epsilonSymbol);

            insertRange(rhs, elements_to_insert);
        }

        if (index == index_maximum && firstSet[back_element].contains(epsilonSymbol)) {
            rhs.emplace(epsilonSymbol);
        }

        return insertRange(firstSet.at(key), rhs);
    }

    auto evaluateFirstSet(Symbol epsilon, const GrammarRulesStorage &rules)
        -> std::map<Symbol, std::set<Symbol>>
    {
        auto first_set = detail::FirstSetEvaluator(epsilon, rules);
        return std::move(first_set.getFirstSet());
    }
}// namespace ccl::parser