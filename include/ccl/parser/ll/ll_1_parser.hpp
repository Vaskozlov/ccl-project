#ifndef CCL_PROJECT_LL_1_PARSER_HPP
#define CCL_PROJECT_LL_1_PARSER_HPP

#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/ll/ll_parser_generator.hpp>

namespace ccl::parser
{
    class Ll1Parser
    {
    private:
        ankerl::unordered_dense::map<TableEntry, const Rule *> table;
        std::function<std::string(SmallId)> idToStringConverter;
        const GrammarStorage &storage;
        SmallId grammarGoalSymbol;

    public:
        Ll1Parser(
            SmallId start_symbol, const GrammarStorage &grammar_storage,
            std::function<std::string(SmallId)> id_to_string_converter);

        auto parse(typename ccl::lexer::LexicalAnalyzer::Tokenizer &tokenizer) -> ast::UnNodePtr;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_LL_1_PARSER_HPP */
