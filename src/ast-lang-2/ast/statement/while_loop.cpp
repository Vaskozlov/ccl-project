#include <ast-lang-2/ast/statement/while_loop.hpp>

namespace astlang2::ast::statement
{
    WhileLoop::WhileLoop(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , conditionNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(1))}
      , bodyNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(3))}
    {}

    auto WhileLoop::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        while (true) {
            core::ComputationResult condition_result = conditionNode->compute(interpreter);

            if (condition_result.controlflowStatus == core::ControlflowStatus::RETURN) {
                return condition_result;
            }

            const auto condition_to_bool =
                interpreter.callFunction("bool", {std::move(condition_result.value)});

            if (!*static_cast<bool *>(condition_to_bool.object.get())) {
                return core::ComputationResult{
                    .value =
                        Value{
                            .object = nullptr,
                            .type = interpreter.getVoid(),
                        },
                };
            }

            core::ComputationResult body_result = bodyNode->compute(interpreter);

            if (body_result.controlflowStatus == core::ControlflowStatus::RETURN) {
                return body_result;
            }
        }
    }

    auto WhileLoop::castChildren(const ConversionTable &conversion_table) -> void
    {
        conditionNode->cast(conversion_table);
        bodyNode->cast(conversion_table);
    }

    auto WhileLoop::optimize() -> core::SharedNode<>
    {
        auto new_condition = conditionNode->optimize();
        auto new_body = bodyNode->optimize();

        if (new_condition != nullptr) {
            conditionNode = isl::staticPointerCast<AstlangNode>(std::move(new_condition));
        }

        if (new_body != nullptr) {
            bodyNode = isl::staticPointerCast<AstlangNode>(std::move(new_body));
        }

        return nullptr;
    }
}// namespace astlang2::ast::statement
