#ifndef CCL_PROJECT_NODE_HPP
#define CCL_PROJECT_NODE_HPP

#include <ccl/lex/token.hpp>
#include <iostream>
#include <string>

#define ValueExpressionConstructor(Name, Id)                                                       \
    explicit Name(parser::NodePtr value_) : parser::ValueExpression<Id>(std::move(value_))         \
    {}

#define UnaryExpressionConstructor(Name, Id)                                                       \
    Name(parser::NodePtr value_, parser::NodePtr operation_)                                       \
      : parser::UnaryExpression<Id>(std::move(value_), std::move(operation_))                      \
    {}

#define BinaryExpressionConstructor(Name, Id)                                                      \
    Name(parser::NodePtr right_, parser::NodePtr operation_, parser::NodePtr left_)                \
      : parser::BinaryExpression<Id>(std::move(right_), std::move(operation_), std::move(left_))   \
    {}

namespace ccl::parser
{
    struct Node;
    using NodePtr = std::unique_ptr<Node>;

    struct Node
    {
        explicit Node(size_t id_) : id(id_)
        {}

        Node(const Node &) = default;
        Node(Node &&) noexcept = default;
        virtual ~Node() = default;

        auto operator=(const Node &) -> Node & = default;
        auto operator=(Node &&) noexcept -> Node & = default;

        [[nodiscard]] auto getId() const noexcept -> size_t
        {
            return id;
        }

        static auto getPrintingPrefix(const std::string &prefix, bool is_left) -> std::string
        {
            return prefix + (is_left ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500");
        }

        static auto
            expandPrefix(const std::string &prefix, bool is_left, size_t extra_expansion = 0)
                -> std::string
        {
            auto result = prefix + (is_left ? "\u2502   " : "    ");

            for (size_t i = 0; i != extra_expansion; ++i) {
                result.append("   ");
            }

            return result;
        }

        virtual auto print(const std::string &prefix = "", bool is_left = false) const -> void;

    private:
        size_t id{};
    };

    struct TokenNode : Node
    {
        CCL_PERFECT_FORWARDING(T, lex::Token)// NOLINTNEXTLINE
        explicit TokenNode(T &&token_) : Node(token_.getId()), token(std::forward<T>(token_))
        {}

        auto print(const std::string &prefix, bool is_left) const -> void override;

    private:
        lex::Token token{};
    };

    template<size_t Id>
    struct ValueExpression : Node
    {
        explicit ValueExpression(NodePtr value_) : Node(Id), value(std::move(value_))
        {}

        auto print(const std::string &prefix, bool is_left) const -> void override
        {
            fmt::print("{}Value expression\n", getPrintingPrefix(prefix, is_left));
            value->print(expandPrefix(prefix, is_left), false);
        }

        NodePtr value;
    };

    template<size_t Id>
    struct UnaryExpression : Node
    {
        explicit UnaryExpression(NodePtr operation_, NodePtr value_)
          : Node(Id), operation(std::move(operation_)), value(std::move(value_))
        {}

        auto print(const std::string &prefix, bool is_left) const -> void override
        {
            fmt::print("{}Unary expression\n", getPrintingPrefix(prefix, is_left));
            operation->print(expandPrefix(prefix, is_left), true);
            value->print(expandPrefix(prefix, is_left), false);
        }

        NodePtr operation;
        NodePtr value;
    };

    template<size_t Id>
    struct BinaryExpression : Node
    {
        explicit BinaryExpression(NodePtr rhs_, NodePtr operation_, NodePtr lhs_)
          : Node(Id), operation(std::move(operation_)), lhs(std::move(lhs_)), rhs(std::move(rhs_))
        {}

        auto print(const std::string &prefix, bool is_left) const -> void override
        {
            fmt::print("{}Binary expression\n", getPrintingPrefix(prefix, is_left));
            operation->print(expandPrefix(prefix, is_left), false);
            lhs->print(expandPrefix(prefix, is_left, 1), true);
            rhs->print(expandPrefix(prefix, is_left, 1), false);
        }

        NodePtr operation;
        NodePtr lhs;
        NodePtr rhs;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_NODE_HPP */
