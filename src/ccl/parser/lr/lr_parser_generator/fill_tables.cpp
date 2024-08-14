#include "ccl/parser/lr/detail/lr_parser_generator.hpp"
#include <ranges>

namespace ccl::parser
{
    auto LrParserGenerator::getLrActionTable() const
        -> ankerl::unordered_dense::map<TableEntry, Action>
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
                            .idToStr = idToStringConverter,
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

    [[nodiscard]] auto LrParserGenerator::getGlrActionTable() const
        -> ankerl::unordered_dense::map<TableEntry, std::vector<Action>>
    {
        auto result = ankerl::unordered_dense::map<TableEntry, std::vector<Action>>{};

        for (const auto &[key, actions] : actionTable) {
            result.try_emplace(key, actions.begin(), actions.end());
        }

        return result;
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

    auto LrParserGenerator::fillActionTableEntry(
        const CanonicalCollection &cc,
        const GrammarSlot &item) -> void
    {
        using enum ccl::parser::ParsingAction;

        if (item.isDotInTheEnd() && item.getProductionType() == goalProduction) {
            insertIntoActionTable(
                TableEntry{
                    .state = cc.id,
                    .symbol = endOfInput,
                },
                std::monostate{});
        } else if (item.isDotInTheEnd()) {
            insertIntoActionTable(
                TableEntry{
                    .state = cc.id,
                    .symbol = item.getLookAhead(),
                },
                item);
        } else {
            const auto symbol_at_dot = item.at(item.getDotLocation());

            if (!isTerminal(symbol_at_dot)) {
                return;
            }

            const auto entry = TableEntry{
                .state = cc.id,
                .symbol = symbol_at_dot,
            };

            insertIntoActionTable(entry, transitions.at(entry));
        }
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

    template<typename... Ts>
    auto LrParserGenerator::insertIntoActionTable(TableEntry entry, Ts &&...args) -> void
    {
        auto action = Action{std::forward<Ts>(args)...};

        actionTable.try_emplace(entry);
        actionTable.at(entry).emplace(std::move(action));
    }

    auto LrParserGenerator::insertIntoGotoTable(TableEntry entry, State state) -> void
    {
        auto [it, inserted] = gotoTable.try_emplace(entry, state);

        if (!inserted) {
            throw std::logic_error("Goto table conflict");
        }
    }
}// namespace ccl::parser
