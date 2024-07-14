#include "ccl/parser/first_set.hpp"
#include "ccl/parser/lr_parser.hpp"

namespace ccl::parser
{
    LrParser::LrParser(
        const LrItem &start_item, Id epsilon_symbol, isl::Set<Id> grammar_symbols,
        isl::Set<Id> terminal_symbols, isl::Map<Id, isl::Vector<isl::Vector<Id>>> parser_rules)
      : rules{std::move(parser_rules)}
      , allSymbols{std::move(grammar_symbols)}
      , terminals{std::move(terminal_symbols)}
      , goalProduction{start_item.getProductionType()}
      , endOfInput{start_item.getLookAhead()}
      , epsilon{epsilon_symbol}
      , firstSet{evaluateFirstSet(epsilon, allSymbols, terminals, rules)}
    {
        constructCanonicalCollection(start_item);
        fillTables();
    }
}// namespace ccl::parser