#include "ccl/parser/first_set.hpp"
#include "ccl/lexer/token.hpp"

namespace ccl::parser
{
    detail::FirstSetEvaluator::FirstSetEvaluator(
        Symbol epsilon_symbol, const isl::Set<Symbol> &grammar_symbols,
        const isl::Set<Symbol> &terminal_symbols,
        const isl::Map<Symbol, isl::Vector<isl::Vector<Symbol>>> &parser_rules)
      : FirstAndFollowSetsCommon{grammar_symbols, terminal_symbols, parser_rules}
      , epsilon{epsilon_symbol}
    {
        initializeFirstSet();
        computeFirstSet();
    }

    auto detail::FirstSetEvaluator::initializeFirstSet() -> void
    {
        for (auto symbol : allSymbolsInGrammar) {
            if (!lexer::isUsedDefinedTokenOrEoF(symbol)) {
                continue;
            }

            if (isTerminal(symbol)) {
                firstSet.try_emplace(symbol, isl::Set<Symbol>{symbol});
            } else {
                firstSet.try_emplace(symbol);
            }
        }
    }

    auto detail::FirstSetEvaluator::computeFirstSet() -> void
    {
        applyFixedPointAlgorithmOnAllRules([this](Symbol key, const isl::Vector<Symbol> &rule) {
            return firstSetComputationIteration(key, rule);
        });
    }

    auto detail::FirstSetEvaluator::firstSetComputationIteration(
        Symbol key, const isl::Vector<Symbol> &rule) -> bool
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
        Symbol epsilon, const isl::Set<Symbol> &grammar_symbols, const isl::Set<Symbol> &terminals,
        const isl::Map<Symbol, isl::Vector<isl::Vector<Symbol>>> &rules)
        -> isl::Map<Symbol, isl::Set<Symbol>>
    {
        auto first_set = detail::FirstSetEvaluator(epsilon, grammar_symbols, terminals, rules);
        return std::move(first_set.getFirstSet());
    }
}// namespace ccl::parser