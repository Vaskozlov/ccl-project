#ifndef CCL_PROJECT_LEXER_RULE_OPTIONS_HPP
#define CCL_PROJECT_LEXER_RULE_OPTIONS_HPP

#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleOptions final : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        auto construct(ParserBuilder &parser_builder) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_OPTIONS_HPP */
