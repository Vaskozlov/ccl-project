#include "ccl/parser/first_set.hpp"
#include "ccl/parser/lr_parser.hpp"

namespace ccl::parser
{
    LrParser::LrParser(
        const LrItem &start_item, Symbol epsilon_symbol, isl::Set<Symbol> grammar_symbols,
        isl::Set<Symbol> terminal_symbols, GrammarRulesStorage parser_rules)
      : grammarRules{std::move(parser_rules)}
      , allSymbols{std::move(grammar_symbols)}
      , terminalSymbols{std::move(terminal_symbols)}
      , goalProduction{start_item.getProductionType()}
      , endOfInput{start_item.getLookAhead()}
      , epsilonSymbol{epsilon_symbol}
      , firstSet{evaluateFirstSet(epsilonSymbol, allSymbols, terminalSymbols, grammarRules)}
    {
        constructCanonicalCollection(start_item);
        fillTables();
    }
}// namespace ccl::parser