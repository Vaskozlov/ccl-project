#ifndef AST_LANG_2_IF_STATEMENT_HPP
#define AST_LANG_2_IF_STATEMENT_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::statement {
    class IfStatement final : public AstlangNode {
    private:
        isl::SmallVector<SharedNode<AstlangNode>, 3> nodes;

    public:
        IfStatement(SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes);

        auto compute(interpreter::Interpreter&interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const -> ChildrenNodesGenerator override;
    };
} // namespace astlang2::ast::statement

#endif /* AST_LANG_2_IF_STATEMENT_HPP */
