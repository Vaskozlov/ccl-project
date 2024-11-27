#ifndef AST_LANG_2_WHILE_LOOP_HPP
#define AST_LANG_2_WHILE_LOOP_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::statement
{
    class WhileLoop final : public AstlangNode
    {
    private:
        SharedNode<AstlangNode> conditionNode;
        SharedNode<AstlangNode> bodyNode;

    public:
        WhileLoop(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const -> ChildrenNodesGenerator override;
    };
}// namespace astlang2::ast::statement

#endif /* AST_LANG_2_WHILE_LOOP_HPP */
