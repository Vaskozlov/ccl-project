#ifndef AST_LANG_2_FOR_LOOP_HPP
#define AST_LANG_2_FOR_LOOP_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::statement {
    class ForLoop final : public AstlangNode {
    public:
        using AstlangNode::AstlangNode;

        ForLoop(SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes);

        auto compute(interpreter::Interpreter&interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        [[nodiscard]] auto getChildrenNodes() const -> ChildrenNodesGenerator override;
    };
} // namespace astlang2::ast::statement

#endif /* AST_LANG_2_FOR_LOOP_HPP */
