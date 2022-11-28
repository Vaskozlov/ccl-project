#ifndef CCL_PROJECT_PARSER_TYPES_HPP
#define CCL_PROJECT_PARSER_TYPES_HPP

#include <ccl/parser/node_templates.hpp>

namespace ccl::parser
{
    using Stack = SmallVector<UniquePtr<Node>>;

    enum struct RuleOnStackResult : Id
    {
        NO_MATCH,
        FULL_MATCH,
        PARTIAL_MATCH,
        PRECEDENCE_FORBIDDEN
    };

    enum struct ParsingRuleType : Id
    {
        EOI = 0,
        BAD_TOKEN = 1,
        ROOT = 2
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSER_TYPES_HPP */
