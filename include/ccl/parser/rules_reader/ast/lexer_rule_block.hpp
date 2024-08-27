#ifndef CCL_PROJECT_LEXER_RULE_BLOCK_HPP
#define CCL_PROJECT_LEXER_RULE_BLOCK_HPP

#include <ccl/lexer/rule/union.hpp>
#include <ccl/parser/ast/terminal.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_options.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleBlock : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        [[nodiscard]] auto getValue() const -> const parser::ast::Terminal *;

        [[nodiscard]] auto getOptions() const -> const LexerRuleOptions *;

        auto construct(ParserBuilder &parser_builder) const -> isl::UniqueAny override;

        auto applyOptions(
            ParserBuilder &parser_builder,
            lexer::rule::RuleBlockInterface *rule_block) const -> void;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_BLOCK_HPP */
