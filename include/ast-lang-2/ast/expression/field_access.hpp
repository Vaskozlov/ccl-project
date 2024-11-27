#ifndef AST_LANG_2_FIELD_ACCESS_HPP
#define AST_LANG_2_FIELD_ACCESS_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::expression
{
    class FieldAccess final : public AstlangNode
    {
    private:
        SharedNode<AstlangNode> objectNode;
        std::string fieldName;

    public:
        FieldAccess(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const -> ChildrenNodesGenerator override;
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_FIELD_ACCESS_HPP */
