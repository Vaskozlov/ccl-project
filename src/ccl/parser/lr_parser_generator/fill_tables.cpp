#include "ccl/parser/lr/detail//lr_parser_generator.hpp"
#include <ranges>

namespace ccl::parser
{
    auto LrParserGenerator::getLrActionTable() const -> std::map<TableEntry, Action>
    {
        auto has_errors = false;
        auto result = std::map<TableEntry, Action>{};

        for (const auto &[key, actions] : actionTable) {
            if (actions.size() != 1) {
                fmt::println("State: {}", key.state);
                fmt::println("Lookahead: {}", idToStringConverter(key.lookAhead));

                for (const auto &action : actions) {
                    fmt::println("Action: {}", ActionPrintWrapper(action, idToStringConverter));
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

    [[nodiscard]] auto
        LrParserGenerator::getGlrActionTable() const -> std::map<TableEntry, std::vector<Action>>
    {
        auto result = std::map<TableEntry, std::vector<Action>>{};

        for (const auto &[key, actions] : actionTable) {
            result.try_emplace(key, std::vector<Action>{actions.begin(), actions.end()});
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

    auto LrParserGenerator::fillActionTableEntry(const CanonicalCollection &cc, const LrItem &item)
        -> void
    {
        using enum ccl::parser::ParsingAction;

        if (item.isDotInTheEnd() && item.getProductionType() == goalProduction) {
            insertIntoActionTable(
                TableEntry{
                    .state = cc.id,
                    .lookAhead = endOfInput,
                },
                std::monostate{});
        } else if (item.isDotInTheEnd()) {
            insertIntoActionTable(
                TableEntry{
                    .state = cc.id,
                    .lookAhead = item.getLookAhead(),
                },
                item);
        } else {
            auto entry = TableEntry{
                .state = cc.id,
                .lookAhead = item.at(item.getDotLocation()),
            };

            insertIntoActionTable(entry, transitions.at(entry));
        }
    }

    auto LrParserGenerator::fillGotoTableEntry(const CanonicalCollection &cc, Symbol symbol) -> void
    {
        auto entry = TableEntry{.state = cc.id, .lookAhead = symbol};

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
        if (!gotoTable.contains(entry)) {
            gotoTable.try_emplace(entry, state);
            return;
        }

        if (gotoTable.at(entry) != state) {
            throw std::logic_error("Goto table conflict");
        }
    }
}// namespace ccl::parser