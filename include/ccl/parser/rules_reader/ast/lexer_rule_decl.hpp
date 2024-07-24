#ifndef CCL_PROJECT_LEXER_RULE_DECL_HPP
#define CCL_PROJECT_LEXER_RULE_DECL_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleDecl
      : public parser::ast::NodeSequence<isl::UniquePtr>
      , public RulesReaderNode
    {
    public:
        using parser::ast::NodeSequence<isl::UniquePtr>::NodeSequence;

        [[nodiscard]] auto getDeclarationName() const -> isl::string_view;

        [[nodiscard]] auto getRules() const -> const ast::LexerRuleBlock *;

        [[nodiscard]] auto isSpecial() const noexcept -> bool;

        auto construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_DECL_HPP */
