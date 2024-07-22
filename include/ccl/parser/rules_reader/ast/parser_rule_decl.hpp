#ifndef CCL_PROJECT_PARSER_RULE_DECL_HPP
#define CCL_PROJECT_PARSER_RULE_DECL_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>

namespace ccl::parser::reader::ast
{
    class ParserRuleDecl : public parser::ast::UnNodeSequence
    {
    public:
        using parser::ast::UnNodeSequence::NodeSequence;

        [[nodiscard]] auto getDeclarationName() const -> const parser::ast::TokenNode *
        {
            const auto *value_node = this->front();
            return isl::as<const parser::ast::TokenNode *>(value_node);
        }

        [[nodiscard]] auto getRules() const -> const ast::LexerRuleBlock *
        {
            const auto *value_node = this->back();
            return isl::as<const ast::LexerRuleBlock *>(value_node);
        }
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_PARSER_RULE_DECL_HPP */
