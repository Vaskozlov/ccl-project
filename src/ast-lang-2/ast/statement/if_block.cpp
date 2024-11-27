#include <ast-lang-2/ast/statement/if_block.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    IfBlock::IfBlock(const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , conditionNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(1))}
      , bodyNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(3))}
    {}

    auto IfBlock::compute(interpreter::Interpreter &interpreter) const -> ComputationResult
    {
        ComputationResult condition_result = conditionNode->compute(interpreter);
        const Value condition_value = std::move(condition_result.value);

        if (condition_value.type != interpreter.getBool()) {
            throw std::runtime_error{"Condition must be a boolean"};
        }

        const bool condition = *static_cast<const bool *>(condition_value.object.get());

        if (condition) {
            ComputationResult statement_result = bodyNode->compute(interpreter);

            statement_result.controlflowStatus =
                statement_result.controlflowStatus == ControlflowStatus::RETURN
                    ? statement_result.controlflowStatus
                    : ControlflowStatus::IF_BLOCK_FINISHES;

            return statement_result;
        }

        return ComputationResult{
            .controlflowStatus = ControlflowStatus::IF_CONDITION_FAILED,
        };
    }

    auto IfBlock::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(conditionNode, conditionNode->optimize());
        exchangeIfNotNull(bodyNode, bodyNode->optimize());

        return nullptr;
    }

    auto IfBlock::getChildrenNodes() const -> ChildrenNodesGenerator
    {
        return ChildrenNodesGenerator{
            [this, field_index = 0U]() mutable -> ccl::parser::ast::SharedNode<> {
                switch (field_index++) {
                case 0:
                    return conditionNode;

                case 1:
                    return bodyNode;

                default:
                    return nullptr;
                }
            }};
    }
}// namespace astlang2::ast::statement
