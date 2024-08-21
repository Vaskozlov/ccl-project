#ifndef CCL_PROJECT_SPPF_NODE_HPP
#define CCL_PROJECT_SPPF_NODE_HPP

#include <ccl/parser/rule_with_dot.hpp>

namespace ccl::parser
{
    struct SPPFNode
    {
        RuleWithDot rule{};
        SmallId left{};
        SmallId right{};

        [[nodiscard]] auto operator<=>(const SPPFNode &) const -> std::weak_ordering = default;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_SPPF_NODE_HPP */
