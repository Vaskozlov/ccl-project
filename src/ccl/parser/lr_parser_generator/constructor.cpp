#include "ccl/parser/first_set.hpp"
#include "ccl/parser/lr/lr_parser_generator.hpp"

namespace ccl::parser
{
    LrParserGenerator::LrParserGenerator(
        const LrItem &start_item, Symbol epsilon_symbol, const isl::Set<Symbol> &grammar_symbols,
        const isl::Set<Symbol> &terminal_symbols, const GrammarRulesStorage &parser_rules)
      : grammarRules{parser_rules}
      , allSymbols{grammar_symbols}
      , terminalSymbols{terminal_symbols}
      , goalProduction{start_item.getProductionType()}
      , endOfInput{start_item.getLookAhead()}
      , epsilonSymbol{epsilon_symbol}
      , firstSet{evaluateFirstSet(epsilonSymbol, allSymbols, terminalSymbols, grammarRules)}
    {
        constructCanonicalCollection(start_item);
        fillTables();
    }
}// namespace ccl::parser