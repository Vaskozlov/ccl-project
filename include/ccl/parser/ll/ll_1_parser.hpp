#ifndef CCL_PROJECT_LL_1_PARSER_HPP
#define CCL_PROJECT_LL_1_PARSER_HPP

#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/ll/ll_parser_generator.hpp>

namespace ccl::parser::ll
{
    class LL1
    {
    public:
        std::unordered_map<TableEntry, const Rule *> table;
        std::function<std::string(SmallId)> idToStringConverter;
        const GrammarRulesStorage &storage;

        LL1(SmallId start_symbol, const GrammarRulesStorage &grammar_storage,
            std::function<std::string(SmallId)> id_to_string_converter);

        auto parse(Symbol start, typename ccl::lexer::LexicalAnalyzer::Tokenizer &tokenizer)
            -> ast::ShNodePtr;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_1_PARSER_HPP */
