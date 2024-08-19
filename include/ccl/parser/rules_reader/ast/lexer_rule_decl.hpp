#ifndef CCL_PROJECT_LEXER_RULE_DECL_HPP
#define CCL_PROJECT_LEXER_RULE_DECL_HPP

#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleDecl : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        [[nodiscard]] auto getDeclarationName() const -> isl::string_view;

        [[nodiscard]] auto getRules() const -> const LexerRuleBlock *;

        [[nodiscard]] auto isSpecial() const noexcept -> bool;

        auto construct(ParserBuilder &parser_builder) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_DECL_HPP */
