#include <ast-lang-2/ast/statement/elif_block.hpp>

namespace astlang2::ast::statement
{
    static constexpr std::size_t ELIF_BLOCK_SIZE_WITHOUT_NEXT_ELIF = 5;

    auto ElifBlock::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        const auto block_computation_result = IfBlock::compute(interpreter);

        if (block_computation_result.controlflowStatus ==
            core::ControlflowStatus::IF_BLOCK_FINISHES) {
            return block_computation_result;
        }

        if (size() != ELIF_BLOCK_SIZE_WITHOUT_NEXT_ELIF) {
            const auto *next_elif_node = static_cast<const AstlangNode *>(back().get());
            return next_elif_node->compute(interpreter);
        }

        return core::ComputationResult{
            .controlflowStatus = core::ControlflowStatus::IF_CONDITION_FAILED,
        };
    }
}// namespace astlang2::ast::statement
