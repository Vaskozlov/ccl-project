#ifndef AST_LANG_2_FIIELD_ACCESS_HPP
#define AST_LANG_2_FIIELD_ACCESS_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::expression
{
    class FieldAccess final : public core::AstlangNode
    {
    private:
        core::SharedNode<AstlangNode> objectNode;
        std::string fieldName;

    public:
        FieldAccess(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;

        auto castChildren(const ConversionTable &conversion_table) -> void override;

        auto optimize() -> core::SharedNode<> override;
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_FIIELD_ACCESS_HPP */
