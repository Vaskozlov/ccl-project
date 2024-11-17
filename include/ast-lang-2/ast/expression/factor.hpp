#ifndef AST_LANG_2_FACTOR_HPP
#define AST_LANG_2_FACTOR_HPP

#include <ast-lang-2/ast/core/node.hpp>

namespace astlang2::ast::expression
{
    class Factor final : public core::AstlangNode
    {
    private:
        core::SharedNode<> node;
        std::shared_ptr<void> precomputedValue;
        NodeTypes nodeType;

    public:
        Factor(SmallId id, ccl::parser::ast::SmallVectorOfNodes initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;

        auto castChildren(const ConversionTable &conversion_table) -> void override;

        auto optimize() -> core::SharedNode<> override;
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_FACTOR_HPP */
