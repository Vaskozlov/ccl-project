#ifndef AST_LANG_2_FACTOR_HPP
#define AST_LANG_2_FACTOR_HPP

#include <ast-lang-2/ast/core/basic_value.hpp>

namespace astlang2::ast::expression
{
    class Factor final : public core::AstlangNode
    {
    public:
        using AstlangNode::AstlangNode;

        auto compute(interpreter::Interpreter& interpreter) const -> core::ComputationResult override;
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_FACTOR_HPP */
