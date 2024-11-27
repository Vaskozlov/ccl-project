#ifndef AST_LANG_2_FACTOR_HPP
#define AST_LANG_2_FACTOR_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::expression
{
    class Factor final : public AstlangNode
    {
    private:
        SharedNode<> node;
        std::shared_ptr<void> precomputedValue;
        NodeTypes nodeType;

    public:
        Factor(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const -> ChildrenNodesGenerator override;
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_FACTOR_HPP */
