#include <ast-lang-2/ast/statement/else_block.hpp>

namespace astlang2::ast::statement {
    ElseBlock::ElseBlock(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes)
        : AstlangNode{id}
          , bodyNode{isl::staticPointerCast<AstlangNode>(initial_nodes.at(2))} {
    }

    auto ElseBlock::compute(interpreter::Interpreter&interpreter) const -> core::ComputationResult {
        core::ComputationResult statement_result = bodyNode->compute(interpreter);

        statement_result.controlflowStatus = core::ControlflowStatus::IF_BLOCK_FINISHES;
        return statement_result;
    }

    auto ElseBlock::castChildren(const ConversionTable&conversion_table) -> void {
        bodyNode->cast(conversion_table);
    }

    auto ElseBlock::optimize() -> core::SharedNode<> {
        auto new_body = bodyNode->optimize();

        if (new_body != nullptr) {
            bodyNode = bodyNode;
        }

        return nullptr;
    }
} // namespace astlang2::ast::statement
