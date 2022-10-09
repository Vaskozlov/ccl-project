#ifndef CCL_PROJECT_NODE_HPP
#define CCL_PROJECT_NODE_HPP

#include <ccl/lex/token.hpp>
#include <iostream>
#include <string>

#define FactorConstructor(Name, Id)                                                                \
    explicit Name(ccl::UniquePtr<Node> value_) : ccl::parser::Factor(Id, std::move(value_))        \
    {}

#define ValueExpressionConstructor(Name, Id)                                                       \
    explicit Name(ccl::UniquePtr<Node> value_)                                                     \
      : ccl::parser::ValueExpression(Id, std::move(value_))                                        \
    {}

#define UnaryExpressionConstructor(Name, Id)                                                       \
    Name(ccl::UniquePtr<Node> value_, ccl::UniquePtr<Node> operation_)                             \
      : ccl::parser::UnaryExpression(Id, std::move(value_), std::move(operation_))                 \
    {}

#define BinaryExpressionConstructor(Name, Id)                                                      \
    Name(ccl::UniquePtr<Node> right_, ccl::UniquePtr<Node> operation_, ccl::UniquePtr<Node> left_) \
      : ccl::parser::BinaryExpression(                                                             \
            Id, std::move(left_), std::move(operation_), std::move(right_))                        \
    {}

namespace ccl::parser
{
    struct Node
    {
        CCL_INLINE explicit Node(size_t id_) : id(id_)
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

        virtual auto print(const std::string &prefix = "", bool is_left = false) const -> void;

    protected:
        static auto getPrintingPrefix(const std::string &prefix, bool is_left) -> std::string;
        static auto
            expandPrefix(const std::string &prefix, bool is_left, size_t extra_expansion = 0)
                -> std::string;

    private:
        size_t id{};
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_NODE_HPP */
