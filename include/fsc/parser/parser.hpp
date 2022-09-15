#ifndef CCL_PROJECT_FSC_PARSER_HPP
#define CCL_PROJECT_FSC_PARSER_HPP

#include <fsc/lex/lexical_analyzer.hpp>
#include <fsc/parser/ast_node.hpp>
#include <iostream>
#include <memory>

namespace fsc
{
    class Parser
    {
    public:
        using Tokenizer = ccl::lex::LexicalAnalyzer::Tokenizer;

        explicit Parser(Tokenizer &tokenizer_) : tokenizer(tokenizer_)
        {
            parse();
        }

    private:
        auto parse() -> void;

        auto parseFunctionDeclaration() -> unique_ptr<ast::BasicNode>;

        auto parseParameters() -> unique_ptr<ast::ParamsListNode>;

        auto parseExpression() -> unique_ptr<ast::BasicNode>;
        auto parseRhsExpression(size_t precedence, unique_ptr<ast::BasicNode> lhs)
            -> unique_ptr<ast::BasicNode>;

        auto parseTerm() -> unique_ptr<ast::BasicNode>;
        auto termLookAhead() -> unique_ptr<ast::BasicNode>;

        auto badTokenError() -> void;

        Tokenizer &tokenizer;
    };
}// namespace fsc

#endif /* CCL_PROJECT_FSC_PARSER_HPP */
