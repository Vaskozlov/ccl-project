#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    auto LrParserGenerator::doClosureComputationIteration(const LrItem &item) const
        -> isl::Generator<LrItem>
    {
        if (item.isDotInTheEnd()) {
            co_return;
        }

        const auto index = item.getDotLocation();
        const auto symbol = item.at(index);

        if (isTerminal(symbol)) {
            co_return;
        }

        const auto has_symbol_after_current = (index + 1 == item.size());
        const auto next_symbol =
            has_symbol_after_current ? item.getLookAhead() : item.at(index + 1);

        const auto &first_set_of_next_symbol = firstSet.at(next_symbol);

        for (const auto first_symbol : first_set_of_next_symbol) {
            const auto &rules = grammarRules.at(symbol);

            for (const auto &rule : rules) {
                co_yield LrItem{std::addressof(rule), 0, symbol, first_symbol};
            }
        }
    }

    auto LrParserGenerator::computeClosure(std::list<LrItem> s) const -> std::list<LrItem>
    {
        auto has_modifications = true;

        while (has_modifications) {
            has_modifications = false;

            for (const auto &item : s) {
                auto items_generator = doClosureComputationIteration(item);

                for (auto &generated_item : items_generator) {
                    if (std::ranges::find(s, generated_item) != s.end()) {
                        continue;
                    }

                    s.emplace_back(generated_item);
                }
            }
        }

        return s;
    }
}// namespace ccl::parser