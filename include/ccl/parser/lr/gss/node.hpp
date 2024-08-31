#ifndef CCL_PROJECT_LR_GSS_NODE_HPP
#define CCL_PROJECT_LR_GSS_NODE_HPP

#include <ccl/parser/ast/allocator.hpp>

namespace ccl::parser::lr
{
    struct Node
    {
        using PreviousVector = isl::SmallVector<Node *, 4>;

        PreviousVector previous;
        ast::SharedNode<> value{};
        SmallId inputPosition{};
        SmallId parserState{};

        auto addPrevious(Node *parent) -> void
        {
            for (auto *node : previous) {
                if (node == parent) {
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
}// namespace ccl::parser::lr

#endif /* CCL_PROJECT_LR_GSS_NODE_HPP */
