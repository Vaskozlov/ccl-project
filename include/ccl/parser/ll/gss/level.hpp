#ifndef CCL_PROJECT_LL_GSS_LEVEL_HPP
#define CCL_PROJECT_LL_GSS_LEVEL_HPP

#include <ccl/parser/ll/gss/node.hpp>

namespace ccl::parser::ll
{
    struct Level : std::vector<UniqueGssNodePtr>
    {
        using std::vector<UniqueGssNodePtr>::vector;

        [[nodiscard]] auto findNode(const SPPFNode &sppf_node) const CCL_LIFETIMEBOUND -> Node *;

        [[nodiscard]] auto
            findNode(RuleWithDot rule, Symbol production) const CCL_LIFETIMEBOUND -> Node *;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_GSS_LEVEL_HPP */
