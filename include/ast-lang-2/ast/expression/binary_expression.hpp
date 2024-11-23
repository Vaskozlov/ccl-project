#ifndef AST_LANG_2_BINARY_EXPRESSION_HPP
#define AST_LANG_2_BINARY_EXPRESSION_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::expression
{
    class BinaryExpression final : public AstlangNode
    {
    private:
        SharedNode<AstlangNode> lhsNode;
        SharedNode<AstlangNode> rhsNode;
        std::string functionName;

    public:
        BinaryExpression(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()> override;
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_BINARY_EXPRESSION_HPP */
