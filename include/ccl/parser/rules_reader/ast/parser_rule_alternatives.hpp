#ifndef CCL_PROJECT_PARSER_RULE_ALTERNATIVES_HPP
#define CCL_PROJECT_PARSER_RULE_ALTERNATIVES_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_options.hpp>

namespace ccl::parser::reader::ast
{
    class ParserRuleAlternatives : public parser::ast::NodeSequence<isl::UniquePtr>
    {
    public:
        using parser::ast::NodeSequence<isl::UniquePtr>::NodeSequence;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_PARSER_RULE_ALTERNATIVES_HPP */
