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

        auto parseFunctionDeclaration() -> UniquePtr<ast::BasicNode>;

        auto parseParameters() -> UniquePtr<ast::ParamsListNode>;

        auto parseExpression() -> UniquePtr<ast::BasicNode>;
        auto parseRhsExpression(size_t precedence, UniquePtr<ast::BasicNode> lhs)
            -> UniquePtr<ast::BasicNode>;

        auto parseTerm() -> UniquePtr<ast::BasicNode>;
        auto termLookAhead() -> UniquePtr<ast::BasicNode>;

        auto badTokenError() -> void;

        Tokenizer &tokenizer;
    };
}// namespace fsc

#endif /* CCL_PROJECT_FSC_PARSER_HPP */
