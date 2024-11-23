#include <ast-lang-2/ast/statement/else_block.hpp>

namespace astlang2::ast::statement
{
    ElseBlock::ElseBlock(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , bodyNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(2))}
    {}

    auto ElseBlock::compute(interpreter::Interpreter &interpreter) const -> ComputationResult
    {
        ComputationResult statement_result = bodyNode->compute(interpreter);

        statement_result.controlflowStatus = ControlflowStatus::IF_BLOCK_FINISHES;
        return statement_result;
    }

    auto ElseBlock::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(bodyNode, bodyNode->optimize());
        return nullptr;
    }

    auto ElseBlock::getChildrenNodes() const -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()>
    {
        return isl::SmallFunction<ccl::parser::ast::SharedNode<>()>{
            [index = 0, stored_node = bodyNode]() mutable -> ccl::parser::ast::SharedNode<> {
                if (index > 0) {
                    return nullptr;
                }

                ++index;
                return stored_node;
            }};
    }

}// namespace astlang2::ast::statement
