#include <ankerl/unordered_dense.h>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/visualization/graph.hpp>
#include <isl/io.hpp>

namespace ccl::parser::visualization
{
    static constexpr std::string_view DotFileHeader = R"(
graph ""
{
 fontname="Helvetica,Arial,sans-serif"
 node [fontname="Helvetica,Arial,sans-serif"]
 edge [fontname="Helvetica,Arial,sans-serif"]
#   node [fontsize=10,width=".2", height=".2", margin=0];
#   graph[fontsize=8];
label="Test"
)";

    struct VisualizationData
    {
        ankerl::unordered_dense::map<const ast::Node *, std::size_t> nodeToId{};
        std::function<std::string(SmallId)> nodeTypeToString{};
        std::vector<std::pair<std::size_t, std::size_t>> edges{};
        std::vector<std::string> edgesLabels{};
        std::unordered_map<std::size_t, std::string> nodesLabels{};
        std::size_t idGenerator{1};
    };

    static auto addQuotesToEscapingSymbols(std::string_view str) -> std::string
    {
        auto result = std::string{};
        result.reserve(str.size());

        for (auto chr : str) {
            if (chr == '\"') {
                result.append("\\\"");
            } else {
                result.push_back(chr);
            }
        }

        return result;
    }

    static auto visualize(
        const ast::Node *root, std::size_t parent_id, VisualizationData &visualization_data) -> void
    {
        const auto *node_as_sequence = dynamic_cast<const ast::NodeOfNodes *>(root);
        const auto *node_as_token = dynamic_cast<const ast::TokenNode *>(root);
        auto node_label_str = visualization_data.nodeTypeToString(root->getType());

        auto node_id = visualization_data.idGenerator;
        auto node_in_map = visualization_data.nodeToId.find(root);

        if (node_in_map != visualization_data.nodeToId.end()) {
            node_id = node_in_map->second;
        } else {
            visualization_data.nodeToId.try_emplace(root, node_id);
            ++visualization_data.idGenerator;
        }

        if (node_as_token != nullptr) {
            const auto &token = node_as_token->getToken();
            node_label_str += fmt::format(": {}", token.getRepr());
        }

        if (parent_id != 0) {
            visualization_data.edges.emplace_back(parent_id, node_id);
        }

        visualization_data.nodesLabels.try_emplace(node_id, node_label_str);

        if (node_as_sequence != nullptr) {
            for (const auto *node : node_as_sequence->getNodesPointers()) {
                visualize(node, node_id, visualization_data);
            }
        }
    }

    auto visualize(
        const std::filesystem::path &resulted_file_path, const ast::Node *root,
        std::function<std::string(SmallId)> node_type_to_string) -> void
    {
        auto visualization_data =
            VisualizationData{.nodeTypeToString = std::move(node_type_to_string)};

        visualize(root, 0, visualization_data);

        auto result = std::string{DotFileHeader};
        result += fmt::format("n{} ;\n", visualization_data.edges.front().first);

        for (auto [parent, child] : visualization_data.edges) {
            result += fmt::format(" n{} -- n{};\n", parent, child);
        }

        for (auto &[key, repr] : visualization_data.nodesLabels) {
            result += fmt::format(" n{} [label=\"{}\"]\n", key, addQuotesToEscapingSymbols(repr));
        }

        result.append("}\n");
        isl::io::writeToFile(resulted_file_path, result);
    }
}// namespace ccl::parser::visualization
