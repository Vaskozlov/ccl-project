#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser::lr
{
    auto LrParserGenerator::computeClosure(const LrItem &item) const -> const std::vector<LrItem> &
    {
        auto [it, inserted] = closureComputationCache.try_emplace(item);

        if (!inserted) {
            return it->second;
        }

        const auto symbol = item.dottedRule.atDot();
        auto result = std::vector<LrItem>{};

        const auto next_symbol = item.dottedRule.dotPosition + 1 == item.dottedRule.size()
                                     ? item.getLookAhead()
                                     : item.dottedRule.at(item.dottedRule.dotPosition + 1);

        const auto &first_set = grammarRules.getFirstSet();

        for (const auto first_symbol : first_set.at(next_symbol)) {
            for (const auto &rule : grammarRules.at(symbol)) {
                result.emplace_back(
                    RuleWithDot{
                        .rule = std::addressof(rule),
                        .dotPosition = 0,
                    },
                    symbol, first_symbol);
            }
        }

        return it->second = std::move(result);
    }

    auto LrParserGenerator::computeClosureOnItems(std::vector<LrItem> s) const
        -> const std::vector<LrItem> &
    {
        auto [it, inserted] = closureComputationOnItemsCache.try_emplace(s);

        if (!inserted) {
            return it->second;
        }

        auto has_modifications = true;

        const auto is_not_in_set = [&s](const LrItem &slot) {
            return std::ranges::find(s, slot) == s.end();
        };

        while (has_modifications) {
            has_modifications = false;

            for (std::size_t index = 0; index != s.size(); ++index) {
                const auto &item = s.at(index);

                if (item.dottedRule.isDotInTheEnd() || isTerminal(item.dottedRule.atDot())) {
                    continue;
                }

                for (const auto &generated_item :
                     computeClosure(item) | std::views::filter(is_not_in_set)) {
                    s.emplace_back(generated_item);
                }
            }
        }

        return it->second = std::move(s);
    }
}// namespace ccl::parser::lr
