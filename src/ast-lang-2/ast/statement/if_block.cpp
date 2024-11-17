#include <ast-lang-2/ast/statement/if_block.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement {
    IfBlock::IfBlock(const SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes)
        : AstlangNode{id}
          , conditionNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(1))}
          , bodyNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(3))} {
    }

    auto IfBlock::compute(interpreter::Interpreter&interpreter) const -> core::ComputationResult {
        core::ComputationResult condition_result = conditionNode->compute(interpreter);
        const Value condition_value = std::move(condition_result.value);

        if (condition_value.type != interpreter.getBool()) {
            throw std::runtime_error{"Condition must be a boolean"};
        }

        const bool condition = *static_cast<const bool *>(condition_value.object.get());

        if (condition) {
            core::ComputationResult statement_result = bodyNode->compute(interpreter);

            statement_result.controlflowStatus =
                    statement_result.controlflowStatus == core::ControlflowStatus::RETURN
                        ? statement_result.controlflowStatus
                        : core::ControlflowStatus::IF_BLOCK_FINISHES;

            return statement_result;
        }

        return core::ComputationResult{
            .controlflowStatus = core::ControlflowStatus::IF_CONDITION_FAILED,
        };
    }

    auto IfBlock::castChildren(const ConversionTable&conversion_table) -> void {
        conditionNode->cast(conversion_table);
        bodyNode->cast(conversion_table);
    }

    auto IfBlock::optimize() -> core::SharedNode<> {
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
} // namespace astlang2::ast::statement
