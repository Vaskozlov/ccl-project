#ifndef AST_LANG_2_FUNCTION_DEFINITION_HPP
#define AST_LANG_2_FUNCTION_DEFINITION_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::function::def
{
    class FunctionDefinition final : public core::AstlangNode
    {
    private:
        std::string functionName;
        std::vector<std::string> argumentsNames;
        isl::SmallVector<core::SharedNode<ccl::parser::ast::NonTerminal>, 2> functionArguments;
        core::SharedNode<ccl::parser::ast::NonTerminal> returnTypeNode;
        core::SharedNode<AstlangNode> bodyNode;

    public:
        FunctionDefinition(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;

        auto castChildren(const ConversionTable &conversion_table) -> void override;

        auto optimize() -> core::SharedNode<> override;
    };
}// namespace astlang2::ast::function::def

#endif /* AST_LANG_2_FUNCTION_DEFINITION_HPP */
