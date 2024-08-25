#ifndef CCL_PROJECT_ASTLANG_BASIC_VALUE_HPP
#define CCL_PROJECT_ASTLANG_BASIC_VALUE_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::expression
{
    class BasicValue : public Node
    {
    public:
        using Node::Node;

        auto optimize() -> SharedNode<> override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_BASIC_VALUE_HPP */
