#ifndef CCL_PROJECT_NODE_TEMPLATES_HPP
#define CCL_PROJECT_NODE_TEMPLATES_HPP

#include <ccl/parser/node.hpp>

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

    struct TokenNode : Node
    {
        CCL_PERFECT_FORWARDING(T, lex::Token)// NOLINTNEXTLINE
        explicit TokenNode(T &&token_) : Node(token_.getId()), token(std::forward<T>(token_))
        {}

        [[nodiscard]] auto getToken() const noexcept -> const lex::Token &
        {
            return token;
        }

        auto print(const std::string &prefix, bool is_left) const -> void override;

    private:
        lex::Token token{};
    };

    struct Factor : Node
    {
        explicit Factor(size_t id_, UniquePtr<Node> value_);

        auto print(const std::string &prefix, bool is_left) const -> void override;

        UniquePtr<Node> value;
    };

    struct ValueExpression : Node
    {
        explicit ValueExpression(size_t id_, UniquePtr<Node> value_);

        auto print(const std::string &prefix, bool is_left) const -> void override;

        UniquePtr<Node> value;
    };

    struct UnaryExpression : Node
    {
        explicit UnaryExpression(size_t id_, UniquePtr<Node> operation_, UniquePtr<Node> value_);

        auto print(const std::string &prefix, bool is_left) const -> void override;

        UniquePtr<Node> operation;
        UniquePtr<Node> value;
    };

    struct BinaryExpression : Node
    {
        explicit BinaryExpression(
            size_t id_, UniquePtr<Node> lhs_, UniquePtr<Node> operation_, UniquePtr<Node> rhs_);

        auto print(const std::string &prefix, bool is_left) const -> void override;

        UniquePtr<Node> operation;
        UniquePtr<Node> lhs;
        UniquePtr<Node> rhs;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_NODE_TEMPLATES_HPP */
