#include <ast-lang-2/ast/statement/if_statement.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    IfStatement::IfStatement(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
    {
        for (const auto &node : initial_nodes) {
            nodes.emplace_back(isl::staticPointerCast<AstlangNode>(node));
        }
    }

    auto IfStatement::compute(interpreter::Interpreter &interpreter) const
        -> ComputationResult
    {
        for (const auto &node : nodes) {
            ComputationResult result = node->compute(interpreter);

            if (result.controlflowStatus == ControlflowStatus::RETURN) {
                return result;
            }

            if (result.controlflowStatus == ControlflowStatus::IF_BLOCK_FINISHES) {
                return ComputationResult{.value = std::move(result.value)};
            }
        }

        return ComputationResult{
            .value =
                Value{
                    .object = nullptr,
                    .type = interpreter.getVoid(),
                },
        };
    }

    auto IfStatement::optimize() -> SharedNode<>
    {
        for (auto &node : nodes) {
            exchangeIfNotNull(node, node->optimize());
        }

        return nullptr;
    }

    auto IfStatement::getChildrenNodes() const -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()>
    {
        return isl::SmallFunction<ccl::parser::ast::SharedNode<>()>{
            [this, field_index = 0U]() mutable -> ccl::parser::ast::SharedNode<> {
                if (field_index == nodes.size()) {
                    return nullptr;
                }

                return nodes[field_index++];
            }};
    }
}// namespace astlang2::ast::statement
