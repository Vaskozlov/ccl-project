#ifndef CCL_PROJECT_LL_1_PARSER_HPP
#define CCL_PROJECT_LL_1_PARSER_HPP

#include <ccl/parser/ll/ll_1_parser_generator.hpp>
#include <ccl/parser/parsing_result.hpp>

namespace ccl::parser
{
    class Ll1Parser
    {
    private:
        ll::Ll1Table table;
        std::function<std::string(SmallId)> idToStringConverter;
        const GrammarStorage &storage;
        SmallId grammarGoalSymbol;

    public:
        Ll1Parser(
            SmallId start_symbol,
            const GrammarStorage &grammar_storage,
            const std::function<std::string(SmallId)> &id_to_string_converter);

        auto parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) -> UnambiguousParsingResult;
    };
} // namespace ccl::parser

#endif /* CCL_PROJECT_LL_1_PARSER_HPP */
