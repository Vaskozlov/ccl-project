#include <ankerl/unordered_dense.h>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/dot/dot_repr.hpp>

namespace ccl::parser::dot
{
    using Edge = isl::dot::detail::Edge;

    struct EdgeInfo
    {
        std::string label;
        std::string color = "black";
    };

    struct NodeInfo
    {
        std::string label;
        std::string color = "black";
    };

    struct TreeInformation
    {
        ankerl::unordered_dense::map<const ast::Node *, std::size_t> nodeToId{};
        std::function<std::string(SmallId)> nodeTypeToString{};
        ankerl::unordered_dense::set<Edge> edges{};
        ankerl::unordered_dense::map<Edge, EdgeInfo> edgesInfo{};
        std::unordered_map<std::size_t, NodeInfo> nodesInfo{};
        std::size_t idGenerator{1};
    };

    static auto createDotRepresentation(
        const ast::Node *root,
        std::size_t parent_id,
        isl::ssize_t child_index,
        TreeInformation &tree_data) -> void
    {
        const auto *node_as_sequence = dynamic_cast<const ast::NodeOfNodes *>(root);
        const auto *node_as_token = dynamic_cast<const ast::TokenNode *>(root);
        auto node_label_str = tree_data.nodeTypeToString(root->getType());

        auto node_id = tree_data.idGenerator;
        auto node_in_map = tree_data.nodeToId.find(root);

        if (node_in_map != tree_data.nodeToId.end()) {
            node_id = node_in_map->second;
        } else {
            tree_data.nodeToId.try_emplace(root, node_id);
            ++tree_data.idGenerator;
        }

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
            auto nodes = node_as_sequence->getNodesPointers();
            auto number_of_nodes = static_cast<isl::ssize_t>(nodes.size());

            for (isl::ssize_t i = 0; i != number_of_nodes; ++i) {
                createDotRepresentation(
                    nodes.at(i), node_id, number_of_nodes == 1 ? -1 : i, tree_data);
            }
        }
    }

    auto createDotRepresentation(
        const std::vector<const ast::Node *> &roots,
        std::function<std::string(SmallId)>
            node_type_to_string) -> std::string
    {
        auto tree_data = TreeInformation{.nodeTypeToString = std::move(node_type_to_string)};

        for (const auto *root : roots) {
            createDotRepresentation(root, 0, 0, tree_data);
        }

        auto result = std::string{isl::dot::detail::DotFileHeader};

        for (auto edge : tree_data.edges) {
            auto [parent, child] = edge;
            const auto &edge_info = tree_data.edgesInfo.at(edge);

            result += fmt::format(
                " n{} -> n{} [label=\"{}\", color=\"{}\"];\n",
                parent,
                child,
                edge_info.label,
                edge_info.color);
        }

        for (auto &[key, node_info] : tree_data.nodesInfo) {
            result += fmt::format(
                " n{} [color=\"{}\",label=\"{}\"]\n",
                key,
                node_info.color,
                isl::dot::detail::addQuotesToEscapingSymbols(node_info.label));
        }

        result.append("}\n");
        return result;
    }
}// namespace ccl::parser::dot
