#ifndef CCL_PROJECT_PARSER_RULE_BODY_HPP
#define CCL_PROJECT_PARSER_RULE_BODY_HPP

#include <ccl/parser/ast/node_sequence.hpp>

namespace ccl::parser::reader::ast
{
    class ParserRuleBody : public parser::ast::UnNodeSequence
    {
    public:
        using parser::ast::NodeSequence<isl::UniquePtr>::NodeSequence;
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_PARSER_RULE_BODY_HPP */
