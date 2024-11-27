#include <ast-lang-2/ast/statement/statements.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    Statements::Statements(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
    {
        if (initial_nodes.size() == 0) {
            return;
        }

        nodes.emplace_back(isl::staticPointerCast<AstlangNode>(initial_nodes.front()));

        if (initial_nodes.size() == 1) {
            return;
        }

        const auto *statements =
            static_cast<ccl::parser::ast::NonTerminal *>(initial_nodes.back().get());

        while (true) {
            nodes.emplace_back(isl::staticPointerCast<AstlangNode>(statements->front()));

            if (statements->size() == 1) {
                break;
            }

            statements = static_cast<ccl::parser::ast::NonTerminal *>(statements->back().get());
        }
    }

    auto Statements::compute(interpreter::Interpreter &interpreter) const -> ComputationResult
    {
        ComputationResult result{
            .value =
                Value{
                    .object = nullptr,
                    .type = interpreter.getVoid(),
                },
        };

        for (const auto &statement : nodes) {
            result = statement->compute(interpreter);

            if (result.controlflowStatus == ControlflowStatus::RETURN) {
                return result;
            }
        }

        return result;
    }

    auto Statements::optimize() -> SharedNode<>
    {
        for (auto &node : nodes) {
            exchangeIfNotNull(node, node->optimize());
        }

        if (nodes.size() == 1) {
            return nodes.front();
        }

        return nullptr;
    }

    auto Statements::getChildrenNodes() const -> ChildrenNodesGenerator
    {
        return ChildrenNodesGenerator{
            [index = 0U, &children_nodes = nodes]() mutable -> ccl::parser::ast::SharedNode<> {
                if (index == children_nodes.size()) {
                    return nullptr;
                }

                return children_nodes[index++];
            }};
    }
}// namespace astlang2::ast::statement
