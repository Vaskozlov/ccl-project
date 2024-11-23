#ifndef AST_LANG_2_STATEMENT_HPP
#define AST_LANG_2_STATEMENT_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::statement {
    class Statement final : public AstlangNode {
    private:
        SharedNode<AstlangNode> node;

    public:
        Statement(SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes);

        auto compute(interpreter::Interpreter&interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()> override;
    };
} // namespace astlang2::ast::statement

#endif /* AST_LANG_2_STATEMENT_HPP */
