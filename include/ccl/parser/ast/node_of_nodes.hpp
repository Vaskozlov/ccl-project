#ifndef CCL_PROJECT_NODE_OF_NODES_HPP
#define CCL_PROJECT_NODE_OF_NODES_HPP

#include <ccl/parser/ast/node.hpp>
#include <optional>

namespace ccl::parser::ast
{
    class NodeOfNodes : public Node
    {
    public:
        using Node::Node;

        [[nodiscard]] virtual auto getNodesPointers() const -> std::vector<const Node *> = 0;

        auto print(
            const std::string &prefix, bool is_left,
            std::function<std::string(SmallId)> id_converter) const -> void override;
    };
}// namespace ccl::parser::ast


#endif /* CCL_PROJECT_NODE_OF_NODES_HPP */
