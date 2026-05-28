#ifndef AST_LANG_2_FACTOR_HPP
#define AST_LANG_2_FACTOR_HPP

#include <ast-lang-2/ast/node.hpp>

namespace astlang2::ast::expression
{
    class Factor final : public AstlangNode
    {
    private:
        SharedNode<> node;
        AstlangObject<> precomputedValue;
        NodeTypes nodeType;
        bool isConstant{false};

    public:
        Factor(SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes);

        auto compute(interpreter::Interpreter &interpreter) const -> ComputationResult override;

        auto optimize() -> SharedNode<> override;

        auto print(
            const std::string &prefix, bool is_left,
            const std::function<std::string(SmallId)> &id_converter) const -> void override;

        auto getChildrenNodes() const -> ChildrenNodesGenerator override;

        static auto createConstant(NodeTypes type, AstlangObject<> value) -> SharedNode<>;

        [[nodiscard]] auto getNodeType() const noexcept -> NodeTypes { return nodeType; }
        [[nodiscard]] auto getPrecomputedValue() const noexcept -> const AstlangObject<> & { return precomputedValue; }

        auto updateConstant(NodeTypes type, AstlangObject<> value) -> void
        {
            nodeType = type;
            precomputedValue = std::move(value);
            isConstant = true;
        }
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_FACTOR_HPP */
