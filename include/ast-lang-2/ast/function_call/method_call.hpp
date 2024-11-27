#ifndef AST_LANG_2_METHOD_CALL_HPP
#define AST_LANG_2_METHOD_CALL_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::function::call
{
    class MethodCall final : public AstlangNode
    {
    private:
        SharedNode<AstlangNode> objectNode;
        SharedNode<ccl::parser::ast::Terminal> methodNameNode;
        isl::SmallVector<SharedNode<AstlangNode>, 4> functionArgumentsNode;

    public:
        MethodCall(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const -> ChildrenNodesGenerator override;
    };
}// namespace astlang2::ast::function::call

#endif /* AST_LANG_2_METHOD_CALL_HPP */
