#ifndef AST_LANG_2_FUNCTION_CALL_HPP
#define AST_LANG_2_FUNCTION_CALL_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::function::call {
    class FunctionCall final : public core::AstlangNode {
    private:
        core::SharedNode<ccl::parser::ast::Terminal> functionNameNode;
        isl::SmallVector<core::SharedNode<AstlangNode>, 4> functionArgumentsNode;

    public:
        FunctionCall(SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes);

        auto compute(interpreter::Interpreter&interpreter) const
            -> core::ComputationResult override;

        auto castChildren(const ConversionTable&conversion_table) -> void override;

        auto optimize() -> core::SharedNode<> override;
    };
} // namespace astlang2::ast::function::call

#endif /* AST_LANG_2_FUNCTION_CALL_HPP */
