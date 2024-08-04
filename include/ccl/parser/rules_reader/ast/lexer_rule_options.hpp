#ifndef CCL_PROJECT_LEXER_RULE_OPTIONS_HPP
#define CCL_PROJECT_LEXER_RULE_OPTIONS_HPP

#include <ccl/parser/ast/string_node.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleOptions : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        auto construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_OPTIONS_HPP */
