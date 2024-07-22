#include "ccl/parser/first_set.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    LrParserGenerator::LrParserGenerator(
        const LrItem &start_item, Symbol epsilon_symbol, const GrammarRulesStorage &parser_rules)
      : grammarRules{parser_rules}
      , goalProduction{start_item.getProductionType()}
      , endOfInput{start_item.getLookAhead()}
      , epsilonSymbol{epsilon_symbol}
      , firstSet{evaluateFirstSet(epsilonSymbol, grammarRules)}
    {
        constructCanonicalCollection(start_item);
        fillTables();
    }
}// namespace ccl::parser