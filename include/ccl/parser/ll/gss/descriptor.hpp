#ifndef CCL_PROJECT_LL_GSS_DESCRIPTOR_HPP
#define CCL_PROJECT_LL_GSS_DESCRIPTOR_HPP

#include <ccl/parser/ll/gss/node.hpp>

namespace ccl::parser::ll
{
    struct Descriptor
    {
        Node *stack;
        SmallId inputPosition;

        auto operator<=>(const Descriptor &other) const -> std::weak_ordering = default;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_GSS_DESCRIPTOR_HPP */
