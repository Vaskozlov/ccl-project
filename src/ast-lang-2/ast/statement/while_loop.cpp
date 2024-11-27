#include <ast-lang-2/ast/statement/while_loop.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    WhileLoop::WhileLoop(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , conditionNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(1))}
      , bodyNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(3))}
    {}

    auto WhileLoop::compute(interpreter::Interpreter &interpreter) const -> ComputationResult
    {
        while (true) {
            ComputationResult condition_result = conditionNode->compute(interpreter);

            if (condition_result.controlflowStatus == ControlflowStatus::RETURN) {
                return condition_result;
            }

            const auto condition_to_bool =
                interpreter.callFunction("bool", {std::move(condition_result.value)});

            if (!*static_cast<bool *>(condition_to_bool.object.get())) {
                return ComputationResult{
                    .value =
                        Value{
                            .object = nullptr,
                            .type = interpreter.getVoid(),
                        },
                };
            }

            ComputationResult body_result = bodyNode->compute(interpreter);

            if (body_result.controlflowStatus == ControlflowStatus::RETURN) {
                return body_result;
            }
        }
    }

    auto WhileLoop::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(conditionNode, conditionNode->optimize());
        exchangeIfNotNull(bodyNode, bodyNode->optimize());

        return nullptr;
    }

    auto WhileLoop::getChildrenNodes() const -> ChildrenNodesGenerator
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
