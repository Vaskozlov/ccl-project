#include <ccl/parser/ast/node_of_nodes.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/dot/dot_repr.hpp>
#include <isl/dot_repr.hpp>

namespace ccl::parser::dot
{
    using Edge = isl::dot::Edge;

    using EdgeInfo = isl::dot::EdgeInfo;
    using NodeInfo = isl::dot::NodeInfo;

    template<typename T, typename ToStrArg>
    using TreeInformation = isl::dot::TreeInformation<T, ToStrArg>;

    static auto createDotRepresentation(
        ast::Node *root,
        std::size_t parent_id,
        isl::ssize_t child_index,
        TreeInformation<ast::Node *, SmallId> &tree_data) -> void
    {
        const auto *node_as_sequence = dynamic_cast<const ast::NodeOfNodes *>(root);
        const auto *node_as_token = dynamic_cast<const ast::TokenNode *>(root);
        auto node_label_str = tree_data.nodeTypeToString(root->getType());

        auto node_id = tree_data.createIdForNode(root);

        if (node_as_token != nullptr) {
            const auto &token = node_as_token->getToken();
            node_label_str += fmt::format(": {}", token.getRepr());
        }

        auto [it, inserted] =
            tree_data.nodesInfo.try_emplace(node_id, NodeInfo{.label = node_label_str});

        if (!inserted) {
            it->second.color = "red";
        }

        if (parent_id != 0) {
            const auto edge = Edge{parent_id, node_id};

            tree_data.edges.emplace(edge);
            tree_data.edgesInfo.emplace(
                edge,
                EdgeInfo{
                    .label = child_index == -1 ? "" : fmt::to_string(child_index),
                });
        }

        if (node_as_sequence != nullptr && inserted) {
            auto nodes = node_as_sequence->getNodes();
            auto number_of_nodes = static_cast<isl::ssize_t>(nodes.size());

            for (std::size_t i = 0; i != nodes.size(); ++i) {
                createDotRepresentation(
                    nodes.at(i).get(),
                    node_id,
                    number_of_nodes == 1 ? -1 : static_cast<isl::ssize_t>(i),
                    tree_data);
            }
        }
    }

    auto createDotRepresentation(
        const std::vector<ast::Node *> &roots,
        const std::function<std::string(SmallId)> &node_type_to_string) -> std::string
    {
        auto tree_data =
            TreeInformation<ast::Node *, SmallId>{.nodeTypeToString = node_type_to_string};

        for (auto *root : roots) {
            createDotRepresentation(root, 0, 0, tree_data);
        }

        return tree_data.generateDotRepr();
    }
}// namespace ccl::parser::dot
