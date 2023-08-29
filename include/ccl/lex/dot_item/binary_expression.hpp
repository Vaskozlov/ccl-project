#ifndef CCL_PROJECT_BINARY_EXPRESSION_HPP
#define CCL_PROJECT_BINARY_EXPRESSION_HPP

#include <ccl/lex/dot_item/item_concept.hpp>

namespace ccl::lex::dot_item
{

    enum struct BinaryOperator : u16
    {
        NONE,
        AND,
        OR
    };

    class BinaryExpression final : public DotItemConcept
    {
        using typename DotItemConcept::TextIterator;

        DotItem rhsItem;
        DotItem lhsItem;
        BinaryOperator binaryOperator;

    public:
        [[nodiscard]] auto empty() const noexcept -> bool override;

        [[nodiscard]] BinaryExpression(
            DotItem lhs, DotItem rhs, BinaryOperator binary_operator, Id item_id);

        [[nodiscard]] auto getLhs() const CCL_LIFETIMEBOUND->const DotItem &
        {
            return lhsItem;
        }

        [[nodiscard]] auto getRhs() const CCL_LIFETIMEBOUND->const DotItem &
        {
            return rhsItem;
        }

    private:
        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> Optional<size_t> override;

        [[nodiscard]] auto orIteration(const ForkedGenerator &text_iterator) const
            -> Optional<size_t>;

        [[nodiscard]] auto andIteration(const ForkedGenerator &text_iterator) const
            -> Optional<size_t>;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_BINARY_EXPRESSION_HPP */
