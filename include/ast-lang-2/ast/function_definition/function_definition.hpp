#ifndef AST_LANG_2_FUNCTION_DEFINITION_HPP
#define AST_LANG_2_FUNCTION_DEFINITION_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::function::def
{
    class FunctionDefinition final : public AstlangNode
    {
    private:
        std::string functionName;
        isl::SmallVector<std::string, 2> argumentsNames;
        isl::SmallVector<SharedNode<ccl::parser::ast::NonTerminal>, 2> functionArguments;
        SharedNode<ccl::parser::ast::NonTerminal> returnTypeNode;
        SharedNode<AstlangNode> bodyNode;

    public:
        FunctionDefinition(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto getChildrenNodes() const -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()> override;
    };
}// namespace astlang2::ast::function::def

#endif /* AST_LANG_2_FUNCTION_DEFINITION_HPP */
