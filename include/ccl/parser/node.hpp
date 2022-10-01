#ifndef CCL_PROJECT_NODE_HPP
#define CCL_PROJECT_NODE_HPP

#include <ccl/lex/token.hpp>
#include <iostream>
#include <string>

#define FactorConstructor(Name, Id)                                                                \
    explicit Name(parser::UniquePtr<Node> value_) : parser::Factor<Id>(std::move(value_))          \
    {}

#define ValueExpressionConstructor(Name, Id)                                                       \
    explicit Name(parser::UniquePtr<Node> value_) : parser::ValueExpression<Id>(std::move(value_)) \
    {}

#define UnaryExpressionConstructor(Name, Id)                                                       \
    Name(parser::UniquePtr<Node> value_, parser::UniquePtr<Node> operation_)                       \
      : parser::UnaryExpression<Id>(std::move(value_), std::move(operation_))                      \
    {}

#define BinaryExpressionConstructor(Name, Id)                                                      \
    Name(                                                                                          \
        parser::UniquePtr<Node> left_, parser::UniquePtr<Node> operation_,                         \
        parser::UniquePtr<Node> right_)                                                            \
      : parser::BinaryExpression<Id>(std::move(left_), std::move(operation_), std::move(right_))   \
    {}

namespace ccl::parser
{
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

        static auto getPrintingPrefix(const std::string &prefix, bool is_left) -> std::string;
        static auto
            expandPrefix(const std::string &prefix, bool is_left, size_t extra_expansion = 0)
                -> std::string;

        virtual auto clone() const -> UniquePtr<Node> = 0;
        virtual auto print(const std::string &prefix = "", bool is_left = false) const -> void;

    private:
        size_t id{};
    };

    struct TokenNode : Node
    {
        CCL_PERFECT_FORWARDING(T, lex::Token)// NOLINTNEXTLINE
        explicit TokenNode(T &&token_) : Node(token_.getId()), token(std::forward<T>(token_))
        {}

        [[nodiscard]] auto clone() const -> UniquePtr<Node> override
        {
            return makeUnique<Node, TokenNode>(token);
        }

        [[nodiscard]] auto getToken() const noexcept -> const lex::Token &
        {
            return token;
        }

        auto print(const std::string &prefix, bool is_left) const -> void override;

    private:
        lex::Token token{};
    };

    template<size_t Id>
    struct Factor : Node
    {
        explicit Factor(UniquePtr<Node> value_) : Node(Id), value(std::move(value_))
        {}

        [[nodiscard]] auto clone() const -> UniquePtr<Node> override
        {
            return makeUnique<Node, Factor<Id>>(value->clone());
        }

        auto print(const std::string &prefix, bool is_left) const -> void override
        {
            fmt::print("{}Factor\n", getPrintingPrefix(prefix, is_left));
            value->print(expandPrefix(prefix, is_left), false);
        }

        UniquePtr<Node> value;
    };

    template<size_t Id>
    struct ValueExpression : Node
    {
        explicit ValueExpression(UniquePtr<Node> value_) : Node(Id), value(std::move(value_))
        {}

        [[nodiscard]] auto clone() const -> UniquePtr<Node> override
        {
            return makeUnique<Node, ValueExpression<Id>>(value->clone());
        }

        auto print(const std::string &prefix, bool is_left) const -> void override
        {
            fmt::print("{}Value expression\n", getPrintingPrefix(prefix, is_left));
            value->print(expandPrefix(prefix, is_left), false);
        }

        UniquePtr<Node> value;
    };

    template<size_t Id>
    struct UnaryExpression : Node
    {
        explicit UnaryExpression(UniquePtr<Node> operation_, UniquePtr<Node> value_)
          : Node(Id), operation(std::move(operation_)), value(std::move(value_))
        {}

        [[nodiscard]] auto clone() const -> UniquePtr<Node> override
        {
            return makeUnique<Node, UnaryExpression<Id>>(operation->clone(), value->clone());
        }

        auto print(const std::string &prefix, bool is_left) const -> void override
        {
            fmt::print("{}Unary expression\n", getPrintingPrefix(prefix, is_left));
            operation->print(expandPrefix(prefix, is_left), true);
            value->print(expandPrefix(prefix, is_left), false);
        }

        UniquePtr<Node> operation;
        UniquePtr<Node> value;
    };

    template<size_t Id>
    struct BinaryExpression : Node
    {
        explicit BinaryExpression(
            UniquePtr<Node> lhs_, UniquePtr<Node> operation_, UniquePtr<Node> rhs_)
          : Node(Id), operation(std::move(operation_)), lhs(std::move(lhs_)), rhs(std::move(rhs_))
        {}

        [[nodiscard]] auto clone() const -> UniquePtr<Node> override
        {
            return makeUnique<Node, BinaryExpression<Id>>(
                lhs->clone(), operation->clone(), rhs->clone());
        }

        auto print(const std::string &prefix, bool is_left) const -> void override
        {
            fmt::print("{}Binary expression\n", getPrintingPrefix(prefix, is_left));
            operation->print(expandPrefix(prefix, is_left), false);
            lhs->print(expandPrefix(prefix, is_left, 1), true);
            rhs->print(expandPrefix(prefix, is_left, 1), false);
        }

        UniquePtr<Node> operation;
        UniquePtr<Node> lhs;
        UniquePtr<Node> rhs;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_NODE_HPP */
