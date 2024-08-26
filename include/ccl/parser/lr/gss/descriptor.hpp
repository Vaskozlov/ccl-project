#ifndef CCL_PROJECT_LR_GSS_DESCRIPTOR_HPP
#define CCL_PROJECT_LR_GSS_DESCRIPTOR_HPP

#include <ccl/parser/table_entry.hpp>

namespace ccl::parser::lr
{
    struct Node;

    struct Descriptor
    {
        Node *stack;
        SmallId inputPosition;
        SmallId parserState;

        auto operator<=>(const Descriptor &other) const -> std::weak_ordering = default;
    };
}// namespace ccl::parser::lr

template<>
struct ankerl::unordered_dense::hash<ccl::parser::lr::Descriptor>
{
    using is_avalanching = void;

    [[nodiscard]] auto
        operator()(const ccl::parser::lr::Descriptor &descriptor) const noexcept -> auto
    {
        return detail::wyhash::hash(&descriptor, sizeof(ccl::parser::lr::Descriptor));
    }
};

#endif /* CCL_PROJECT_LR_GSS_DESCRIPTOR_HPP */
