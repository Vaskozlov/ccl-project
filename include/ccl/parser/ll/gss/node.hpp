#ifndef CCL_PROJECT_LL_GSS_NODE_HPP
#define CCL_PROJECT_LL_GSS_NODE_HPP

#include <ccl/parser/ll/sppf_node.hpp>

namespace ccl::parser::ll
{
    struct Node
    {
        using Vector = isl::SmallVector<Node *, 6>;

        Vector previous;
        SPPFNode sppfNode;
        SmallId inputPosition{};

        auto addParent(Node *parent) -> void
        {
            for (const auto *ptr : previous) {
                if (ptr == parent) {
                    return;
                }
            }

            previous.emplace_back(parent);
        }
    };

    namespace detail
    {
        // NOLINTNEXTLINE
        extern isl::FixedSizeAllocator<sizeof(Node), alignof(Node)> NodeAllocator;
    }// namespace detail

    using UniqueGssNodePtr = isl::UniquePtr<Node, std::addressof(detail::NodeAllocator)>;
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_GSS_NODE_HPP */
