#ifndef CCL_PROJECT_NODE_HPP
#define CCL_PROJECT_NODE_HPP

#include <ccl/ccl.hpp>
#include <ccl/lexer/token.hpp>
#include <functional>

namespace ccl::parser::ast
{
    class Node
    {
    private:
        Id nodeType{};

    protected:
        static auto getPrintingPrefix(const std::string &prefix, bool is_left) -> std::string;
        static auto expandPrefix(
            const std::string &prefix, bool is_left, size_t extra_expansion = 1) -> std::string;

    public:
        Node() = default;

        explicit Node(Id node_type)
          : nodeType{node_type}
        {}

        Node(const Node &node) = default;
        Node(Node &&) noexcept = default;

        virtual ~Node() = default;

        auto operator=(const Node &node) -> Node & = default;
        auto operator=(Node &&) noexcept -> Node & = default;

        [[nodiscard]] auto getType() const noexcept -> Id
        {
            return nodeType;
        }

        virtual auto print(
            const std::string &prefix = "", bool is_left = false,
            std::function<isl::string_view(Id)> id_converter = [](auto /* arg */) {
                return "not set";
            }) const -> void = 0;
    };

    using NodePtr = std::unique_ptr<Node>;
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_NODE_HPP */
