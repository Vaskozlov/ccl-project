#ifndef CCL_PROJECT_NODE_SEQUENCE_HPP
#define CCL_PROJECT_NODE_SEQUENCE_HPP

#include <algorithm>
#include <ccl/parser/ast/node_of_nodes.hpp>
#include <optional>
#include <ranges>

namespace ccl::parser::ast
{
    template<template<class> class SmartPointer>
    class NodeSequence : public NodeOfNodes
    {
    private:
        isl::Vector<SmartPointer<Node>> nodes;

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

        explicit NodeSequence(Id node_type_id, isl::Vector<SmartPointer<Node>> initial_nodes)
          : NodeOfNodes{node_type_id}
          , nodes{std::move(initial_nodes)}
        {}

        auto addNode(SmartPointer<Node> node) -> void
        {
            nodes.emplace_back(std::move(node));
        }

        [[nodiscard]] auto getNodes() -> isl::Vector<SmartPointer<Node>> &
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

        [[nodiscard]] auto front() CCL_LIFETIMEBOUND -> Node *
        {
            return nodes.front().get();
        }

        [[nodiscard]] auto front() const CCL_LIFETIMEBOUND -> const Node *
        {
            return nodes.front().get();
        }

        [[nodiscard]] auto back() CCL_LIFETIMEBOUND -> Node *
        {
            return nodes.back().get();
        }

        [[nodiscard]] auto back() const CCL_LIFETIMEBOUND -> const Node *
        {
            return nodes.back().get();
        }

        auto joinWithNode(SmartPointer<Node> node) -> void
        {
            if (auto *casted = isl::as<NodeSequence<SmartPointer> *>(node.get());
                casted != nullptr) {
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

    using UnNodeSequence = NodeSequence<isl::UniquePtr>;
    using ShNodeSequence = NodeSequence<isl::SharedPtr>;
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_NODE_SEQUENCE_HPP */
