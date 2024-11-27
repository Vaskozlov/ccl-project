#ifndef CCL_PROJECT_NODE_HPP
#define CCL_PROJECT_NODE_HPP

#include <ccl/ccl.hpp>
#include <functional>
#include <isl/small_function.hpp>

namespace ccl::parser::ast
{
    class NonTerminal;

    namespace detail
    {
        using SharedPtrFrame = isl::SharedPtrFrame<256, 8>;

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        extern isl::FixedSizeAllocator<sizeof(SharedPtrFrame), alignof(SharedPtrFrame)>
            NodeAllocator;
    }// namespace detail

    class Node;

    template<std::derived_from<Node> T = Node>
    using SharedNode =
        isl::SharedPtr<T, detail::SharedPtrFrame, std::addressof(detail::NodeAllocator)>;

    class Node
    {
    private:
        SmallId nodeType{};

    protected:
        [[nodiscard]] static auto getPrintingPrefix(const std::string &prefix, bool is_left)
            -> std::string;

        [[nodiscard]] static auto
            expandPrefix(const std::string &prefix, bool is_left, std::size_t extra_expansion = 1)
                -> std::string;

    public:
        using ConversionTable = ankerl::unordered_dense::map<SmallId, void (*)(NonTerminal *)>;

        Node() = default;

        explicit Node(const SmallId node_type)
          : nodeType{node_type}
        {}

        Node(const Node &node) = default;

        Node(Node &&) noexcept = default;

        virtual ~Node() = default;

        auto operator=(const Node &node) -> Node & = default;

        auto operator=(Node &&) noexcept -> Node & = default;

        [[nodiscard]] auto getType() const noexcept -> SmallId
        {
            return nodeType;
        }

        virtual auto print(
            const std::string &prefix = "", bool is_left = false,
            const std::function<std::string(SmallId)> &id_converter = fmt::to_string<SmallId>) const
            -> void = 0;

        [[nodiscard]] virtual auto getChildrenNodes() const
            -> isl::SmallFunction<SharedNode<>()> = 0;

        auto castChildren(const ConversionTable &conversion_table) -> void;

        auto cast(const ConversionTable &conversion_table) -> void;
    };
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_NODE_HPP */
