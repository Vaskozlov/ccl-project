#include <ast-lang-2/ast/statement/for_loop.hpp>

namespace astlang2::ast::statement
{
    ForLoop::ForLoop(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes & /*initial_nodes */)
      : AstlangNode{id}
    {}


    auto ForLoop::compute(interpreter::Interpreter & /*interpreter*/) const
        -> ComputationResult
    {
        return {};
    }

    auto ForLoop::optimize() -> SharedNode<>
    {
        return nullptr;
    }

    auto ForLoop::getChildrenNodes() const -> ChildrenNodesGenerator
    {
        return ChildrenNodesGenerator{
            []() -> ccl::parser::ast::SharedNode<> {
                return nullptr;
            }};
    }
}// namespace astlang2::ast::statement
