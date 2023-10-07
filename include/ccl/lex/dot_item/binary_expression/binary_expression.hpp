#ifndef CCL_PROJECT_BINARY_EXPRESSION_HPP
#define CCL_PROJECT_BINARY_EXPRESSION_HPP

#include "ccl/lex/dot_item/item_concept.hpp"

namespace ccl::lex::dot_item
{
    class BinaryOr;
    class BinaryAnd;

    enum struct BinaryOperator : u16
    {
        NONE,
        AND,
        OR
    };

    class BinaryExpression : public DotItemConcept
    {
    private:
        friend BinaryOr;
        friend BinaryAnd;
        using typename DotItemConcept::TextIterator;

        DotItem rhsItem;
        DotItem lhsItem;

    public:
        [[nodiscard]] BinaryExpression(DotItem lhs, DotItem rhs, Id item_id);

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
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_BINARY_EXPRESSION_HPP */
