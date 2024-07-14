#include "ccl/parser/lr_parser.hpp"

namespace ccl::parser
{
    auto LrParser::fillTablesUsingCanonicalCollection(const CanonicalCollection &cc) -> void
    {
        for (const auto &item : cc.items) {
            fillActionTableEntry(cc, item);
        }

        for (const auto product : allSymbols) {
            if (isTerminal(product)) {
                continue;
            }

            fillGotoTableEntry(cc, product);
        }
    }

    auto LrParser::fillActionTableEntry(const CanonicalCollection &cc, const LrItem &item) -> void
    {
        using enum ccl::parser::ParsingAction;

        if (item.isDotInTheEnd() && item.getProductionType() == goalProduction) {
            insertIntoActionTable(
                TableEntry{
                    .state = cc.collectionId,
                    .lookAhead = endOfInput,
                },
                std::monostate{});
        } else if (item.isDotInTheEnd()) {
            insertIntoActionTable(
                TableEntry{
                    .state = cc.collectionId,
                    .lookAhead = item.getLookAhead(),
                },
                item);
        } else {
            auto entry = TableEntry{
                .state = cc.collectionId,
                .lookAhead = item.at(item.getDotLocation()),
            };

            insertIntoActionTable(entry, transitions.at(entry));
        }
    }

    auto LrParser::fillGotoTableEntry(const CanonicalCollection &cc, std::size_t product) -> void
    {
        auto entry = TableEntry{.state = cc.collectionId, .lookAhead = product};

        if (!transitions.contains(entry)) {
            return;
        }

        gotoTable.try_emplace(entry, transitions.at(entry));
    }

    auto LrParser::fillTables() -> void
    {
        for (const auto &cc : canonicalCollection) {
            fillTablesUsingCanonicalCollection(cc);
        }
    }

    template<typename... Ts>
    auto LrParser::insertIntoActionTable(TableEntry entry, Ts &&...args) -> void
    {
        auto action = Action{std::forward<Ts>(args)...};

        if (!actionTable.contains(entry)) {
            actionTable.try_emplace(entry, std::move(action));
            return;
        }

        if (actionTable.at(entry) != action) {
            if (action.getParsingAction() != actionTable.at(entry).getParsingAction()) {
                throw std::logic_error("Shift reduce conflict");
            }

            throw std::logic_error("Reduce reduce conflict");
        }
    }
}// namespace ccl::parser