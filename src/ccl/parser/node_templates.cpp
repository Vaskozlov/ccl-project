#include <ccl/parser/node_templates.hpp>

namespace ccl::parser
{
    Factor::Factor(Id id_, UniquePtr<Node> value_)
      : Node{id_}
      , value{std::move(value_)}
    {}

    ValueExpression::ValueExpression(Id id_, UniquePtr<Node> value_)
      : Node{id_}
      , value{std::move(value_)}
    {}

    UnaryExpression::UnaryExpression(Id id_, UniquePtr<Node> operation_, UniquePtr<Node> value_)
      : Node{id_}
      , operation{std::move(operation_)}
      , value{std::move(value_)}
    {}

    BinaryExpression::BinaryExpression(
        Id id_, UniquePtr<Node> lhs_, UniquePtr<Node> operation_, UniquePtr<Node> rhs_)
      : Node{id_}
      , operation{std::move(operation_)}
      , lhs{std::move(lhs_)}
      , rhs{std::move(rhs_)}
    {}

    auto TokenNode::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}{}\n", getPrintingPrefix(prefix, is_left), token.getRepr());
    }

    auto Factor::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}Factor\n", getPrintingPrefix(prefix, is_left));

        value->print(expandPrefix(prefix, is_left), false);
    }

    auto ValueExpression::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}Value expression\n", getPrintingPrefix(prefix, is_left));

        value->print(expandPrefix(prefix, is_left), false);
    }

    auto UnaryExpression::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}Unary expression\n", getPrintingPrefix(prefix, is_left));

        operation->print(expandPrefix(prefix, is_left), true);
        value->print(expandPrefix(prefix, is_left), false);
    }

    auto BinaryExpression::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}Binary expression\n", getPrintingPrefix(prefix, is_left));

        operation->print(expandPrefix(prefix, is_left), false);
        lhs->print(expandPrefix(prefix, is_left, 1), true);
        rhs->print(expandPrefix(prefix, is_left, 1), false);
    }
}// namespace ccl::parser
