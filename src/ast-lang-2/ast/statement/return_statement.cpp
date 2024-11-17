#include <ast-lang-2/ast/statement/return_statement.hpp>

namespace astlang2::ast::statement {
    ReturnStatement::ReturnStatement(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes)
        : AstlangNode{id}
          , node{isl::staticPointerCast<AstlangNode>(initial_nodes.back())} {
    }

    auto ReturnStatement::compute(interpreter::Interpreter&interpreter) const
        -> core::ComputationResult {
        core::ComputationResult evaluation_result = node->compute(interpreter);
        evaluation_result.controlflowStatus = core::ControlflowStatus::RETURN;
        return evaluation_result;
    }

    auto ReturnStatement::castChildren(const ConversionTable&conversion_table) -> void {
        node->cast(conversion_table);
    }

    auto ReturnStatement::optimize() -> core::SharedNode<> {
        auto new_node = node->optimize();

        if (new_node != nullptr) {
            node = isl::staticPointerCast<AstlangNode>(std::move(new_node));
        }

        return nullptr;
    }
} // namespace astlang2::ast::statement
