#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser::lr
{
    auto LrParserGenerator::getLrActionTable() const -> Lr1ActionTable
    {
        auto has_errors = false;
        auto result = ankerl::unordered_dense::map<TableEntry, Action>{};

        for (const auto &[key, actions] : actionTable) {
            if (actions.size() != 1) {
                fmt::println("State: {}", key.state);
                fmt::println("Lookahead: {}", idToStringConverter(key.symbol));

                for (const auto &action : actions) {
                    fmt::println(
                        "Action: {}",
                        ActionPrintWrapper{
                            .action = action,
                            .idToStringConverter = idToStringConverter,
                        });
                }

                has_errors = true;
            }

            result.try_emplace(key, *actions.begin());
        }

        if (has_errors) {
            throw std::logic_error("Grammar is not suitable for LR parser");
        }

        return result;
    }

    [[nodiscard]] auto LrParserGenerator::getGlrActionTable() const -> const GlrActionTable &
    {
        return actionTable;
    }

    auto
        LrParserGenerator::fillTablesUsingCanonicalCollection(const CanonicalCollection &cc) -> void
    {
        for (const auto &item : cc.items) {
            fillActionTableEntry(cc, item);
        }

        for (const auto symbol : grammarRules.getNonTerminals()) {
            fillGotoTableEntry(cc, symbol);
        }
    }

    auto LrParserGenerator::fillActionTableEntry(const CanonicalCollection &cc, const LrItem &item)
        -> void
    {
        using enum ParsingAction;
        const auto is_dot_in_the_end = item.dottedRule.isDotInTheEnd();

        if (is_dot_in_the_end && item.getProductionType() == goalProduction) {
            insertIntoActionTable(
                {
                    .state = cc.id,
                    .symbol = endOfInput,
                },
                {
                    .parsingAction = ACCEPT,
                });
            return;
        }

        if (is_dot_in_the_end) {
            insertIntoActionTable(
                TableEntry{
                    .state = cc.id,
                    .symbol = item.getLookAhead(),
                },
                {
                    .parsingAction = REDUCE,
                    .productionType = item.production,
                    .numberOfElementsInProduction = static_cast<SmallId>(item.dottedRule.size()),
                });
            return;
        }

        const auto symbol_at_dot = item.dottedRule.atDot();

        if (!isTerminal(symbol_at_dot)) {
            return;
        }

        const auto entry = TableEntry{
            .state = cc.id,
            .symbol = symbol_at_dot,
        };

        insertIntoActionTable(
            entry,
            {
                .parsingAction = SHIFT,
                .shiftingState = transitions.at(entry),
            });
    }

    auto LrParserGenerator::fillGotoTableEntry(const CanonicalCollection &cc, Symbol symbol) -> void
    {
        const auto entry = TableEntry{
            .state = cc.id,
            .symbol = symbol,
        };

        if (!transitions.contains(entry)) {
            return;
        }

        insertIntoGotoTable(entry, transitions.at(entry));
    }

    auto LrParserGenerator::fillTables() -> void
    {
        for (const auto &cc : canonicalCollection) {
            fillTablesUsingCanonicalCollection(cc);
        }
    }

    auto LrParserGenerator::insertIntoActionTable(TableEntry entry, Action action) -> void
    {
        auto [it, inserted] = actionTable.try_emplace(entry);

        if (auto &actions = it->second; std::ranges::find(actions, action) == actions.end()) {
            actions.emplace_back(action);
        }
    }

    auto LrParserGenerator::insertIntoGotoTable(TableEntry entry, State state) -> void
    {
        if (auto [it, inserted] = gotoTable.try_emplace(entry, state); !inserted) {
            throw std::logic_error("Goto table conflict");
        }
    }
}// namespace ccl::parser::lr
