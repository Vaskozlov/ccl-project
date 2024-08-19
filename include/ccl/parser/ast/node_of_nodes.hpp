#ifndef CCL_PROJECT_NODE_OF_NODES_HPP
#define CCL_PROJECT_NODE_OF_NODES_HPP

#include <ccl/parser/ast/node.hpp>
#include <optional>

namespace ccl::parser::ast
{
    class NodeOfNodes : public Node
    {
    protected:
        std::vector<Node *> nodes;

    public:
        explicit NodeOfNodes(SmallId node_type_id)
          : Node{node_type_id}
        {}

        explicit NodeOfNodes(SmallId node_type_id, std::vector<Node *> initial_nodes)
          : Node{node_type_id}
          , nodes{std::move(initial_nodes)}
        {}

        auto print(
            const std::string &prefix, bool is_left,
            std::function<std::string(SmallId)> id_converter) const -> void override;

        [[nodiscard]] virtual auto getNodesPointers() const -> std::vector<const Node *>
        {
            return {nodes.begin(), nodes.end()};
        }

        [[nodiscard]] auto getNodes() -> auto &
        {
            return nodes;
        }

        auto addNode(Node *node) -> void
        {
            nodes.emplace_back(node);
        }

        [[nodiscard]] auto size() const noexcept -> std::size_t
        {
            return nodes.size();
        }

        [[nodiscard]] auto empty() const noexcept -> bool
        {
            return size() == 0;
        }

        [[nodiscard]] auto at(std::size_t index) -> Node *
        {
            return nodes.at(index);
        }

        [[nodiscard]] auto at(std::size_t index) const -> const Node *
        {
            return nodes.at(index);
        }

        [[nodiscard]] auto front() CCL_LIFETIMEBOUND -> Node *
        {
            return nodes.front();
        }

        [[nodiscard]] auto front() const CCL_LIFETIMEBOUND -> const Node *
        {
            return nodes.front();
        }

        [[nodiscard]] auto back() CCL_LIFETIMEBOUND -> Node *
        {
            return nodes.back();
        }

        [[nodiscard]] auto back() const CCL_LIFETIMEBOUND -> const Node *
        {
            return nodes.back();
        }

        auto joinWithNode(Node *node) -> void
        {
            if (auto *casted = isl::as<NodeOfNodes *>(node); casted != nullptr) {
                for (auto *elem : casted->nodes) {
                    addNode(elem);
                }
            } else {
                addNode(node);
            }
        }

        auto reverse() -> void
        {
            std::ranges::reverse(nodes);
        }
    };
}// namespace ccl::parser::ast


#endif /* CCL_PROJECT_NODE_OF_NODES_HPP */
