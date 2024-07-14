#include "ccl/parser/lr_parser.hpp"

namespace ccl::parser
{
    auto LrParser::doClosureComputationIteration(isl::Set<LrItem> &s, const LrItem &item) -> bool
    {
        if (item.isDotInTheEnd()) {
            return false;
        }

        auto has_modifications = false;
        const auto index = item.getDotLocation();
        const auto production = item.at(index);

        if (isTerminal(production)) {
            return false;
        }

        const auto has_production_after_current = (index + 1 == item.length());
        const auto next_production =
            has_production_after_current ? item.getLookAhead() : item.at(index + 1);

        const auto &first_set_of_next_production = firstSet.at(next_production);

        for (const auto first_symbol : first_set_of_next_production) {
            for (const auto &alternatives : rules.at(production)) {
                auto [it, has_inserted] = s.emplace(alternatives, 0, production, first_symbol);
                has_modifications = has_modifications || has_inserted;
            }
        }

        return has_modifications;
    }

    auto LrParser::computeClosure(isl::Set<LrItem> s) -> isl::Set<LrItem>
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