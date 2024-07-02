#ifndef CCL_PROJECT_NODE_SEQUENCE_HPP
#define CCL_PROJECT_NODE_SEQUENCE_HPP

#include <ccl/parser/ast/node.hpp>
#include <optional>

namespace ccl::parser::ast
{
    class NodeSequence : public Node
    {
    private:
        isl::Vector<std::unique_ptr<Node>> nodes;

    public:
        explicit NodeSequence(Id node_type_id)
          : Node{node_type_id}
        {}

        auto print(
            const std::string &prefix, bool is_left,
            std::function<isl::string_view(Id)> id_converter) const -> void override;

        CCL_DECL auto addNode(std::unique_ptr<Node> node)
        {
            nodes.emplace_back(std::move(node));
        }

        CCL_DECL auto getNodes() -> isl::Vector<std::unique_ptr<Node>> &
        {
            return nodes;
        }

        CCL_DECL auto size() const noexcept -> std::size_t
        {
            return nodes.size();
        }

        CCL_DECL auto empty() const noexcept -> bool
        {
            return size() == 0;
        }

        CCL_DECL auto at(std::size_t index) -> std::optional<Node *>
        {
            if (nodes.size() <= index) {
                return std::nullopt;
            }

            return nodes.at(index).get();
        }

        CCL_DECL auto at(std::size_t index) const -> std::optional<const Node *>
        {
            if (nodes.size() <= index) {
                return std::nullopt;
            }

            return nodes.at(index).get();
        }

        auto joinSequences(std::unique_ptr<Node> node) -> void
        {
            if (auto *casted = isl::as<NodeSequence *>(node.get()); casted != nullptr) {
                for (auto &elem : casted->nodes) {
                    addNode(std::move(elem));
                }
            } else {
                addNode(std::move(node));
            }
        }
    };
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_NODE_SEQUENCE_HPP */
