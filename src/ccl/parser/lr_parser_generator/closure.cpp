#include "ccl/parser/lr_parser_generator.hpp"

namespace ccl::parser
{
    auto LrParserGenerator::doClosureComputationIteration(
        isl::Set<LrItem> &s, const LrItem &item) const -> bool
    {
        if (item.isDotInTheEnd()) {
            return false;
        }

        auto has_modifications = false;
        const auto index = item.getDotLocation();
        const auto symbol = item.at(index);

        if (isTerminal(symbol)) {
            return false;
        }

        const auto has_symbol_after_current = (index + 1 == item.length());
        const auto next_symbol =
            has_symbol_after_current ? item.getLookAhead() : item.at(index + 1);

        const auto &first_set_of_next_symbol = firstSet.at(next_symbol);

        for (const auto first_symbol : first_set_of_next_symbol) {
            for (const auto &alternatives : grammarRules.at(symbol)) {
                auto [it, has_inserted] = s.emplace(alternatives, 0, symbol, first_symbol);
                has_modifications = has_modifications || has_inserted;
            }
        }

        return has_modifications;
    }

    auto LrParserGenerator::computeClosure(isl::Set<LrItem> s) const -> isl::Set<LrItem>
    {
        auto has_modifications = true;

        while (has_modifications) {
            has_modifications = false;

            for (const auto &item : s) {
                has_modifications = doClosureComputationIteration(s, item) || has_modifications;
            }
        }

        return s;
    }
}// namespace ccl::parser