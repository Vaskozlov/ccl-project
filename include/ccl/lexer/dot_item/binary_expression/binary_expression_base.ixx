module;
#include <ccl/defines.hpp>
export module ccl.lexer.dot_item.binary_expression:base_class;

export import isl;
export import ccl.text;
export import ccl.lexer.dot_item.core;

export namespace ccl::lexer::dot_item
{
    class BinaryOperationOr;
    class BinaryOperationAnd;

    enum struct BinaryOperator : u16
    {
        NONE,
        AND,
        OR
    };

    class BinaryExpressionBase : public DotItemConcept
    {
    private:
        friend BinaryOperationOr;
        friend BinaryOperationAnd;
        using typename DotItemConcept::TextIterator;

        DotItem rhsItem;
        DotItem lhsItem;

    public:
        [[nodiscard]] BinaryExpressionBase(DotItem lhs, DotItem rhs, Id item_id)
          : DotItemConcept{item_id}
          , rhsItem{std::move(rhs)}
          , lhsItem{std::move(lhs)}
        {}

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return rhsItem->empty() && lhsItem->empty();
        }

        [[nodiscard]] auto getLhs() const CCL_LIFETIMEBOUND -> const DotItem &
        {
            return lhsItem;
        }

        [[nodiscard]] auto getRhs() const CCL_LIFETIMEBOUND -> const DotItem &
        {
            return rhsItem;
        }
    };
}// namespace ccl::lexer::dot_item
