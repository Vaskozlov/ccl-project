#ifndef AST_LANG_2_BASIC_VALUE_HPP
#define AST_LANG_2_BASIC_VALUE_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::core
{
    class BasicValue : public AstlangNode
    {
    public:
        using AstlangNode::AstlangNode;

        auto optimize() -> SharedNode<> override;
    };
}// namespace astlang2::ast::core

#endif /* AST_LANG_2_BASIC_VALUE_HPP */
