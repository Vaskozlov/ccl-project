#ifndef CCL_PROJECT_LL_GSS_PASSED_DESCRIPTOR_HPP
#define CCL_PROJECT_LL_GSS_PASSED_DESCRIPTOR_HPP

#include <ccl/parser/ll/sppf_node.hpp>

namespace ccl::parser::ll
{
    struct PassedDescriptor
    {
        SPPFNode sppfNode;
        SmallId inputPosition{};

        auto operator<=>(const PassedDescriptor &) const -> std::weak_ordering = default;
    };
}// namespace ccl::parser::ll

template<>
struct ankerl::unordered_dense::hash<ccl::parser::ll::PassedDescriptor>
{
    using is_avalanching = void;

    [[nodiscard]] auto
        operator()(const ccl::parser::ll::PassedDescriptor &descriptor) const noexcept -> auto
    {
        return detail::wyhash::mix(
            descriptor.inputPosition,
            ankerl::unordered_dense::hash<ccl::parser::SPPFNode>{}(descriptor.sppfNode));
    }
};

#endif /* CCL_PROJECT_LL_GSS_PASSED_DESCRIPTOR_HPP */
