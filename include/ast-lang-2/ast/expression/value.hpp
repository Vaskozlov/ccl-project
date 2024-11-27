#ifndef AST_LANG_2_VALUE_HPP
#define AST_LANG_2_VALUE_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::expression
{
    class Value final : public AstlangNode
    {
    private:
        SharedNode<AstlangNode> node;
        std::string functionName;

    public:
        Value(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const -> ChildrenNodesGenerator override;
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_VALUE_HPP */
