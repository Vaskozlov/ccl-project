#ifndef AST_LANG_2_ELIF_BLOCK_HPP
#define AST_LANG_2_ELIF_BLOCK_HPP

#include <ast-lang-2/ast/statement/if_block.hpp>

namespace astlang2::ast::statement {
    class ElifBlock final : public AstlangNode {
        struct ElifStatement {
            SharedNode<AstlangNode> conditionNode;
            SharedNode<AstlangNode> bodyNode;
        };

        isl::SmallVector<ElifStatement, 2> elifStatements;

    public:
        ElifBlock(SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes);

        auto compute(interpreter::Interpreter&interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const  -> ChildrenNodesGenerator override;
    };
} // namespace astlang2::ast::statement

#endif /* AST_LANG_2_ELIF_BLOCK_HPP */
