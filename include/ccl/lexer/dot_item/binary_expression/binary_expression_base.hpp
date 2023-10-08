#ifndef CCL_PROJECT_BINARY_EXPRESSION_BASE_HPP
#define CCL_PROJECT_BINARY_EXPRESSION_BASE_HPP

#include <ccl/lexer/dot_item/item_concept.hpp>

namespace ccl::lexer::dot_item
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
        [[nodiscard]] BinaryExpressionBase(DotItem lhs, DotItem rhs, Id item_id);

        [[nodiscard]] auto empty() const noexcept -> bool override;

        [[nodiscard]] auto getLhs() const CCL_LIFETIMEBOUND->const DotItem &
        {
            return lhsItem;
        }

        [[nodiscard]] auto getRhs() const CCL_LIFETIMEBOUND->const DotItem &
        {
            return rhsItem;
        }
    };
}// namespace ccl::lexer::dot_item

#endif /* CCL_PROJECT_BINARY_EXPRESSION_BASE_HPP */
