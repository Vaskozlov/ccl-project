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
            const auto entry = TableEntry{cc.collectionId, endOfInput};
            checkForConflictsInActionTable(entry, ACCEPT);
            actionTable.try_emplace(entry, std::monostate{});
        } else if (item.isDotInTheEnd()) {
            auto entry = TableEntry{cc.collectionId, item.getLookAhead()};
            checkForConflictsInActionTable(entry, REDUCE);
            actionTable.try_emplace(entry, item);
        } else {
            auto entry = TableEntry{cc.collectionId, item.at(item.getDotLocation())};
            checkForConflictsInActionTable(entry, SHIFT);
            actionTable.try_emplace(entry, transitions.at(entry));
        }
    }

    auto LrParser::fillGotoTableEntry(const CanonicalCollection &cc, std::size_t product) -> void
    {
        auto entry = TableEntry{cc.collectionId, product};

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

    auto LrParser::checkForConflictsInActionTable(
        const TableEntry &entry,
        ParsingAction parsingAction) -> void
    {
        if (actionTable.contains(entry) &&
            actionTable.at(entry).getParsingAction() != parsingAction) {
            throw std::logic_error("Shift reduce conflict");
        }
    }
}// namespace ccl::parser