#ifndef CCL_PROJECT_PARSER_TYPES_HPP
#define CCL_PROJECT_PARSER_TYPES_HPP

#include <ccl/parser/node_templates.hpp>

namespace ccl::parser
{
    using Stack = SmallVector<UniquePtr<Node>>;

    CCL_ENUM(// NOLINTNEXTLINE
        RuleOnStackResult, Id, NO_MATCH, FULL_MATCH, PARTIAL_MATCH, PRECEDENCE_FORBIDION);

    CCL_ENUM(// NOLINTNEXTLINE
        ParsingRuleType, Id, EOI, BAD_TOKEN, ROOT);
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSER_TYPES_HPP */
