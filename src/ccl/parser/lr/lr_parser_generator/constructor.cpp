#include "ccl/parser/first_set.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser::lr
{
    LrParserGenerator::LrParserGenerator(
        const LrItem &start_item, const Symbol epsilon_symbol, const GrammarStorage &parser_rules,
        const std::function<std::string(SmallId)> &id_to_string_converter)
      : idToStringConverter{id_to_string_converter}
      , grammarRules{parser_rules}
      , goalProduction{start_item.getProductionType()}
      , endOfInput{start_item.getLookAhead()}
      , epsilonSymbol{epsilon_symbol}
    {
        constructCanonicalCollection(start_item);
        fillTables();
    }
}// namespace ccl::parser::lr
