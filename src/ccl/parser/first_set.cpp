#include "ccl/parser/first_set.hpp"
#include "ccl/lexer/token.hpp"

namespace ccl::parser
{
    FirstSetEvaluator::FirstSetEvaluator(const Symbol epsilon_symbol, GrammarStorage &parser_rules)
      : FirstAndFollowSetsCommon{parser_rules}
      , epsilonSymbol{epsilon_symbol}
    {
        initializeFirstSet();
        computeFirstSet();
    }

    auto FirstSetEvaluator::initializeFirstSet() -> void
    {
        for (auto symbol : grammarRules.getGrammarSymbols()) {
            if (!lexer::isUserDefinedTokenOrEoF(symbol)) {
                continue;
            }

            if (isTerminal(symbol)) {
                firstSet.try_emplace(symbol, std::unordered_set<Symbol>{symbol});
            } else {
                firstSet.try_emplace(symbol);
            }
        }
    }

    auto FirstSetEvaluator::computeFirstSet() -> void
    {
        applyFixedPointAlgorithmOnAllRules([this](const Symbol key, Rule &rule) {
            return firstSetComputationIteration(key, rule);
        });
    }

    auto FirstSetEvaluator::firstSetComputationIteration(const Symbol key, Rule &rule) -> bool
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

        rule.setFirstSet(rhs);
        return insertRange(firstSet.at(key), rhs);
    }
}// namespace ccl::parser