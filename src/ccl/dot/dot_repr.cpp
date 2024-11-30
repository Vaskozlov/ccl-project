#include <ccl/parser/ast/non_terminal.hpp>
#include <ccl/parser/ast/terminal.hpp>
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
        const isl::ssize_t child_index,
        TreeInformation<ast::Node *, SmallId> &tree_data) -> void
    {
        const auto *terminal_node = dynamic_cast<const ast::Terminal *>(root);
        auto node_label_str = tree_data.nodeTypeToString(root->getType());

        auto node_id = tree_data.createIdForNode(root);

        if (terminal_node != nullptr) {
            const auto &token = terminal_node->getToken();
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

        if (!inserted) {
            return;
        }

        const auto nodes_generator = root->getChildrenNodes();
        auto is_single_node = true;

        ast::Node *current_node = nodes_generator().get();
        isl::ssize_t index = 0;

        while (current_node != nullptr) {
            ast::Node *next_node = nodes_generator().get();

            if (next_node != nullptr) {
                is_single_node = false;
            }

            createDotRepresentation(current_node, node_id, is_single_node ? -1 : index, tree_data);

            ++index;
            current_node = next_node;
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
