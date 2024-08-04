#ifndef CCL_PROJECT_LEXER_RULE_BLOCK_HPP
#define CCL_PROJECT_LEXER_RULE_BLOCK_HPP

#include <ccl/lexer/rule/rule_reference.hpp>
#include <ccl/lexer/rule/sequence.hpp>
#include <ccl/lexer/rule/union.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_options.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>
#include <ccl/text/remove_escaping.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleBlock : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        [[nodiscard]] auto getValue() const -> const parser::ast::TokenNode *;

        [[nodiscard]] auto getOptions() const -> const ast::LexerRuleOptions *;

        auto construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny override;

        auto applyOptions(
            RulesConstructor &rule_constructor,
            lexer::rule::RuleBlockInterface *rule_block) const -> void;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_BLOCK_HPP */
