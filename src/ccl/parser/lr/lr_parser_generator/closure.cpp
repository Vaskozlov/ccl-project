#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    static std::size_t counter{};

    auto LrParserGenerator::computeClosure(const LrItem &item) -> const std::vector<LrItem> &
    {
        auto [it, inserted] = closureComputationCache.try_emplace(item);

        if (!inserted) {
            ++counter;
            return it->second;
        }

        const auto index = item.getDotLocation();
        const auto symbol = item.at(index);
        auto result = std::vector<LrItem>{};

        const auto has_symbol_after_current = (index + 1 == item.size());
        const auto next_symbol =
            has_symbol_after_current ? item.getLookAhead() : item.at(index + 1);

        const auto &first_set_of_next_symbol = firstSet.at(next_symbol);

        for (const auto first_symbol : first_set_of_next_symbol) {
            const auto &rules = grammarRules.at(symbol);

            for (const auto &rule : rules) {
                result.emplace_back(std::addressof(rule), 0, symbol, first_symbol);
            }
        }

        it->second = std::move(result);
        return it->second;
    }

    auto LrParserGenerator::computeClosureOnItems(std::vector<LrItem> s)
        -> const std::vector<LrItem> &
    {
        auto [it, inserted] = closureComputationOnItemsCache.try_emplace(s);

        if (!inserted) {
            return it->second;
        }

        auto has_modifications = true;

        while (has_modifications) {
            has_modifications = false;

            for (std::size_t index = 0; index != s.size(); ++index) {
                const auto &item = s.at(index);

                if (item.isDotInTheEnd()) {
                    continue;
                }

                if (isTerminal(item.at(item.getDotLocation()))) {
                    continue;
                }

                const auto &generated_closure = computeClosure(item);

                for (const auto &generated_item : generated_closure) {
                    if (std::ranges::find(s, generated_item) != s.end()) {
                        continue;
                    }

                    s.emplace_back(generated_item);
                }
            }
        }

        it->second = std::move(s);
        return it->second;
    }
}// namespace ccl::parser