#ifndef CCL_PROJECT_NODE_SEQUENCE_HPP
#define CCL_PROJECT_NODE_SEQUENCE_HPP

#include <algorithm>
#include <ccl/parser/ast/node_of_nodes.hpp>
#include <optional>
#include <ranges>

namespace ccl::parser::ast
{
    template<template<class> class T>
    class NodeSequence : public NodeOfNodes
    {
    private:
        isl::Vector<T<Node>> nodes;

        [[nodiscard]] auto getNodes() const -> isl::Vector<const Node *> override
        {
            return isl::Vector<const Node *>{std::views::transform(nodes, [](const auto &node) {
                return node.get();
            })};
        }

    public:
        explicit NodeSequence(Id node_type_id)
          : NodeOfNodes{node_type_id}
        {}

        [[nodiscard]] auto addNode(T<Node> node)
        {
            node.get();
            nodes.emplace_back(std::move(node));
        }

        [[nodiscard]] auto getNodes() -> isl::Vector<T<Node>> &
        {
            return nodes;
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
            return nodes.at(index).get();
        }

        [[nodiscard]] auto at(std::size_t index) const -> const Node *
        {
            return nodes.at(index).get();
        }

        auto joinWithNode(T<Node> node) -> void
        {
            if (auto *casted = isl::as<NodeSequence<T> *>(node.get()); casted != nullptr) {
                for (auto &elem : casted->nodes) {
                    addNode(std::move(elem));
                }
            } else {
                addNode(std::move(node));
            }
        }

        auto reverse() -> void
        {
            std::ranges::reverse(nodes);
        }
    };

    using UnNodeSequence = NodeSequence<std::unique_ptr>;
    using ShNodeSequence = NodeSequence<std::shared_ptr>;
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_NODE_SEQUENCE_HPP */
