#ifndef CCL_PROJECT_PARSER_RULE_DECL_HPP
#define CCL_PROJECT_PARSER_RULE_DECL_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule_alternatives.hpp>

namespace ccl::parser::reader::ast
{
    class ParserRuleDecl : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        [[nodiscard]] auto getDeclarationName() const -> isl::string_view;

        [[nodiscard]] auto getAlternatives() const -> const ast::ParserRuleAlternatives *;

        [[nodiscard]] auto
            construct(ParserBuilder &parser_builder) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_PARSER_RULE_DECL_HPP */
