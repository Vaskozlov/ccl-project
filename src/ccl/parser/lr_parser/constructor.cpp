#include "ccl/parser/first_set.hpp"
#include "ccl/parser/lr_parser.hpp"

namespace ccl::parser
{
    LrParser::LrParser(
        const LrItem &start_item, Production epsilon_symbol, isl::Set<Production> grammar_symbols,
        isl::Set<Production> terminal_symbols,
        isl::Map<Production, isl::Vector<isl::Vector<Production>>> parser_rules)
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