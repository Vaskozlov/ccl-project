#include <ast-lang-2/ast/statement/return_statement.hpp>

namespace astlang2::ast::statement
{
    ReturnStatement::ReturnStatement(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , node{isl::staticPointerCast<AstlangNode>(initial_nodes.back())}
    {}

    auto ReturnStatement::compute(interpreter::Interpreter &interpreter) const
        -> ComputationResult
    {
        ComputationResult evaluation_result = node->compute(interpreter);
        evaluation_result.controlflowStatus = ControlflowStatus::RETURN;
        return evaluation_result;
    }

    auto ReturnStatement::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(node, node->optimize());
        return nullptr;
    }

    auto ReturnStatement::getChildrenNodes() const -> ChildrenNodesGenerator
    {
        return ChildrenNodesGenerator{
            [index = 0, stored_node = node]() mutable -> ccl::parser::ast::SharedNode<> {
                if (index > 0) {
                    return nullptr;
                }

                ++index;
                return stored_node;
            }};
    }

}// namespace astlang2::ast::statement
