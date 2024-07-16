#include "ccl/parser/lr/lr_parser_generator.hpp"

namespace ccl::parser
{
    auto LrParserGenerator::getLrActionTable() const -> isl::Map<TableEntry, Action>
    {
        auto result = isl::Map<TableEntry, Action>{};

        for (const auto &[key, actions] : actionTable) {
            if (actions.size() != 1) {
                throw std::logic_error("Grammar is not suitable for LR parser");
            }

            result.try_emplace(key, *actions.begin());
        }

        return result;
    }

    [[nodiscard]] auto
        LrParserGenerator::getGlrActionTable() -> isl::Map<TableEntry, isl::Vector<Action>>
    {
        auto result = isl::Map<TableEntry, isl::Vector<Action>>{};

        for (const auto &[key, actions] : actionTable) {
            result.try_emplace(key, std::ranges::to<isl::Vector<Action>>(actions));
        }

        return result;
    }

    auto
        LrParserGenerator::fillTablesUsingCanonicalCollection(const CanonicalCollection &cc) -> void
    {
        for (const auto &item : cc.items) {
            fillActionTableEntry(cc, item);
        }

        for (const auto symbol : allSymbols) {
            if (isTerminal(symbol)) {
                continue;
            }

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