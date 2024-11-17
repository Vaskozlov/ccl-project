#include <ast-lang-2/ast/statement/statement.hpp>

namespace astlang2::ast::statement
{
    Statement::Statement(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , node{isl::staticPointerCast<AstlangNode>(initial_nodes.front())}
    {}

    auto Statement::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        return node->compute(interpreter);
    }

    auto Statement::castChildren(const ConversionTable &conversion_table) -> void
    {
        node->cast(conversion_table);
    }

    auto Statement::optimize() -> core::SharedNode<>
    {
        auto new_node = node->optimize();

        if (new_node != nullptr) {
            node = isl::staticPointerCast<AstlangNode>(std::move(new_node));
        }

        return node;
    }
}// namespace astlang2::ast::statement
