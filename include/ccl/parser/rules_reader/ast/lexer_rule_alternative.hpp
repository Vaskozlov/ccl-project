#ifndef CCL_PROJECT_LEXER_RULE_ALTERNATIVE_HPP
#define CCL_PROJECT_LEXER_RULE_ALTERNATIVE_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleAlternative : public RulesReaderNode
    {
    public:
        using RulesReaderNode::RulesReaderNode;

        auto construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny override;
    };
}// namespace ccl::parser::reader::ast


#endif /* CCL_PROJECT_LEXER_RULE_ALTERNATIVE_HPP */
