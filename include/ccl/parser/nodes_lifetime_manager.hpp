#ifndef CCL_PROJECT_NODES_LIFETIME_MANAGER_HPP
#define CCL_PROJECT_NODES_LIFETIME_MANAGER_HPP

#include <ccl/parser/ast/node_of_nodes.hpp>

namespace ccl::parser
{
    class NodesLifetimeManager
    {
    private:
        ast::Node *firstNode{};

    public:
        NodesLifetimeManager() = default;

        NodesLifetimeManager(const NodesLifetimeManager &) = delete;

        NodesLifetimeManager(NodesLifetimeManager &&other) = delete;

        ~NodesLifetimeManager()
        {
            clear();
        }

        auto operator=(const NodesLifetimeManager &) -> NodesLifetimeManager & = delete;

        auto operator=(NodesLifetimeManager &&other) noexcept -> NodesLifetimeManager & = delete;

        auto clear() -> void
        {
            while (firstNode != nullptr) {
                delete std::exchange(firstNode, firstNode->lifetimeManagerPrivateNextNode);
            }
        }

        template<std::derived_from<ast::Node> T = ast::Node, typename... Ts>
        auto create(Ts &&...args) -> T *
        {
            auto *new_node = new T{std::forward<Ts>(args)...};
            new_node->lifetimeManager = this;

            if (firstNode == nullptr) {
                firstNode = new_node;
                return new_node;
            }

            firstNode->lifetimeManagerPrivatePreviousNode = new_node;
            new_node->lifetimeManagerPrivateNextNode = std::exchange(firstNode, new_node);

            return new_node;
        }

        auto remove(const ast::Node *node) -> void
        {
            if (node->lifetimeManagerPrivatePreviousNode != nullptr) {
                node->lifetimeManagerPrivatePreviousNode->lifetimeManagerPrivateNextNode =
                    node->lifetimeManagerPrivateNextNode;
            }

            if (node->lifetimeManagerPrivateNextNode != nullptr) {
                node->lifetimeManagerPrivateNextNode->lifetimeManagerPrivatePreviousNode =
                    node->lifetimeManagerPrivatePreviousNode;
            }

            if (firstNode == node) {
                firstNode = node->lifetimeManagerPrivateNextNode;
            }

            delete node;
        }

        template<std::derived_from<ast::NodeOfNodes> T>
            requires(sizeof(T) == sizeof(ast::NodeOfNodes) &&
                     alignof(T) == alignof(ast::NodeOfNodes) &&
                     std::constructible_from<T, SmallId, std::vector<ast::Node *>>)
        auto promote(ast::NodeOfNodes *node) -> T *
        {
            auto node_type = node->getType();
            auto nodes = std::move(node->getNodes());
            auto *next_node = node->lifetimeManagerPrivateNextNode;
            auto *previous_node = node->lifetimeManagerPrivatePreviousNode;

            std::destroy_at(node);
            node = new (node) T{node_type, std::move(nodes)};

            node->lifetimeManager = this;
            node->lifetimeManagerPrivateNextNode = next_node;
            node->lifetimeManagerPrivatePreviousNode = previous_node;

            return static_cast<T *>(node);
        }

        template<std::derived_from<ast::NodeOfNodes> T>
            requires((sizeof(T) > sizeof(ast::NodeOfNodes) ||
                      alignof(T) != alignof(ast::NodeOfNodes)) &&
                     std::constructible_from<T, SmallId, std::vector<ast::Node *>>)
        auto promote(ast::NodeOfNodes *node) -> T *
        {
            auto node_type = node->getType();
            auto nodes = std::move(node->getNodes());

            auto *next_node = node->lifetimeManagerPrivateNextNode;
            auto *previous_node = node->lifetimeManagerPrivatePreviousNode;

            delete node;
            auto *new_node = new T{node_type, std::move(nodes)};

            new_node->lifetimeManager = this;
            new_node->lifetimeManagerPrivateNextNode = next_node;
            new_node->lifetimeManagerPrivatePreviousNode = previous_node;

            next_node->lifetimeManagerPrivatePreviousNode = new_node;
            previous_node->lifetimeManagerPrivateNextNode = new_node;

            return new_node;
        }

        template<std::derived_from<ast::Node> T = ast::Node>
        auto insert(T *new_node) -> T *
        {
            if (new_node->lifetimeManager == nullptr && firstNode != nullptr) {
                new_node->lifetimeManager = this;
                firstNode->lifetimeManagerPrivatePreviousNode = new_node;
                new_node->lifetimeManagerPrivateNextNode = std::exchange(firstNode, new_node);
            } else if (new_node->lifetimeManager == nullptr) {
                new_node->lifetimeManager = this;
                firstNode = new_node;
            }

            return new_node;
        }
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_NODES_LIFETIME_MANAGER_HPP */
