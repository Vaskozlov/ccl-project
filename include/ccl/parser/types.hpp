#ifndef CCL_PROJECT_PARSER_TYPES_HPP
#define CCL_PROJECT_PARSER_TYPES_HPP

#include <ccl/parser/node.hpp>

namespace ccl::parser
{
    using RuleId = size_t;
    using Stack = SmallVector<UniquePtr<Node>, 4>;

    CCL_ENUM(// NOLINTNEXTLINE
        RuleOnStackResult, size_t, NO_MATCH, FULL_MATCH, PARTIAL_MATCH, PRECEDENCE_FORBIDION);

    CCL_ENUM(// NOLINTNEXTLINE
        ParsingRuleType, RuleId, EOI, BAD_TOKEN, ROOT);
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSER_TYPES_HPP */
