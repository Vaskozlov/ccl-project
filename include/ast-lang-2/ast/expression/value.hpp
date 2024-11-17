#ifndef AST_LANG_2_VALUE_HPP
#define AST_LANG_2_VALUE_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::expression
{
    class Value final : public core::AstlangNode
    {
    private:
        core::SharedNode<AstlangNode> node;
        std::string functionName;

    public:
        Value(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;

        auto castChildren(const ConversionTable &conversion_table) -> void override;

        auto optimize() -> core::SharedNode<> override;
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_VALUE_HPP */
