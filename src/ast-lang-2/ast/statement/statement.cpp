#include <ast-lang-2/ast/statement/statement.hpp>

namespace astlang2::ast::statement
{
    Statement::Statement(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , node{isl::staticPointerCast<AstlangNode>(initial_nodes.front())}
    {}

    auto Statement::compute(interpreter::Interpreter &interpreter) const -> ComputationResult
    {
        return node->compute(interpreter);
    }

    auto Statement::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(node, node->optimize());
        return node;
    }

    auto Statement::getChildrenNodes() const -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()>
    {
        return isl::SmallFunction<ccl::parser::ast::SharedNode<>()>{
            [index = 0, stored_node = node]() mutable -> ccl::parser::ast::SharedNode<> {
                if (index > 0) {
                    return nullptr;
                }

                ++index;
                return stored_node;
            }};
    }
}// namespace astlang2::ast::statement
