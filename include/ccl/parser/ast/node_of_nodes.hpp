#ifndef CCL_PROJECT_NODE_OF_NODES_HPP
#define CCL_PROJECT_NODE_OF_NODES_HPP

#include <ccl/parser/ast/allocator.hpp>
#include <ccl/parser/ast/node.hpp>

namespace ccl::parser::ast
{
    class NodeOfNodes : public Node
    {
    protected:
        std::vector<SharedNode<>> nodes;

    public:
        explicit NodeOfNodes(SmallId node_type_id)
          : Node{node_type_id}
        {}

        explicit NodeOfNodes(SmallId node_type_id, std::vector<SharedNode<>> initial_nodes)
          : Node{node_type_id}
          , nodes{std::move(initial_nodes)}
        {}

        auto print(
            const std::string &prefix, bool is_left,
            std::function<std::string(SmallId)> id_converter) const -> void override;

        [[nodiscard]] auto getNodes() -> auto &
        {
            return nodes;
        }

        [[nodiscard]] auto getNodes() const -> const auto &
        {
            return nodes;
        }

        auto addNode(SharedNode<> node) -> void
        {
            nodes.emplace_back(std::move(node));
        }

        [[nodiscard]] auto size() const noexcept -> std::size_t
        {
            return nodes.size();
        }

        [[nodiscard]] auto empty() const noexcept -> bool
        {
            return size() == 0;
        }

        [[nodiscard]] auto at(std::size_t index) -> SharedNode<> &
        {
            return nodes.at(index);
        }

        [[nodiscard]] auto at(std::size_t index) const -> const SharedNode<> &
        {
            return nodes.at(index);
        }

        [[nodiscard]] auto front() CCL_LIFETIMEBOUND -> SharedNode<> &
        {
            return nodes.front();
        }

        [[nodiscard]] auto front() const CCL_LIFETIMEBOUND -> const SharedNode<> &
        {
            return nodes.front();
        }

        [[nodiscard]] auto back() CCL_LIFETIMEBOUND -> SharedNode<> &
        {
            return nodes.back();
        }

        [[nodiscard]] auto back() const CCL_LIFETIMEBOUND -> const SharedNode<> &
        {
            return nodes.back();
        }

        auto joinWithNode(SharedNode<> node) -> void;

        auto reverse() -> void
        {
            std::ranges::reverse(nodes);
        }

        template<std::constructible_from<SmallId, std::vector<SharedNode<>>> T>
            requires(SharedNode<>::canStore<T>())
        static auto reconstructNode(NodeOfNodes *node) -> void
        {
            auto node_type = node->getType();
            auto nodes = std::move(node->getNodes());

            std::destroy_at(node);
            new (node) T{node_type, std::move(nodes)};
        }

        auto castChildren(const ConversionTable &conversion_table) -> void
        {
            for (auto &node : nodes) {
                cast(conversion_table, node.get());
            }
        }
    };
}// namespace ccl::parser::ast


#endif /* CCL_PROJECT_NODE_OF_NODES_HPP */
