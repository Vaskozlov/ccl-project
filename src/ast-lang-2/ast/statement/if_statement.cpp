#include <ast-lang-2/ast/statement/if_statement.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement {
    IfStatement::IfStatement(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes)
        : AstlangNode{id} {
        for (const auto&node: initial_nodes) {
            nodes.emplace_back(isl::staticPointerCast<AstlangNode>(node));
        }
    }

    auto IfStatement::compute(interpreter::Interpreter&interpreter) const
        -> core::ComputationResult {
        for (const auto&node: nodes) {
            core::ComputationResult result = node->compute(interpreter);

            if (result.controlflowStatus == core::ControlflowStatus::RETURN) {
                return result;
            }

            if (result.controlflowStatus == core::ControlflowStatus::IF_BLOCK_FINISHES) {
                return core::ComputationResult{.value = std::move(result.value)};
            }
        }

        return core::ComputationResult{
            .value =
            Value{
                .object = nullptr,
                .type = interpreter.getVoid(),
            },
        };
    }

    auto IfStatement::castChildren(const ConversionTable&conversion_table) -> void {
        for (auto&node: nodes) {
            node->cast(conversion_table);
        }
    }

    auto IfStatement::optimize() -> core::SharedNode<> {
        for (auto&node: nodes) {
            auto new_node = node->optimize();

            if (new_node != nullptr) {
                node = isl::staticPointerCast<AstlangNode>(std::move(new_node));
            }
        }

        return nullptr;
    }
} // namespace astlang2::ast::statement
