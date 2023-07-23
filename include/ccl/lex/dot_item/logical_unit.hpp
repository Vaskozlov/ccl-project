#ifndef CCL_PROJECT_LOGICAL_UNIT_HPP
#define CCL_PROJECT_LOGICAL_UNIT_HPP

#include <ccl/lex/dot_item/item_concept.hpp>

namespace ccl::lex::dot_item
{

    enum struct LogicalOperation : u16
    {
        NONE,
        AND,
        OR
    };

    class LogicalUnit final : public DotItemConcept
    {
        using typename DotItemConcept::TextIterator;

        DotItem rhsItem;
        DotItem lhsItem;
        LogicalOperation logicalOperation;

    public:
        [[nodiscard]] auto empty() const noexcept -> bool override;

        [[nodiscard]] LogicalUnit(
            DotItem lhs, DotItem rhs, LogicalOperation logical_operation, Id item_id);

        [[nodiscard]] auto getLhs() const -> const DotItem &
        {
            return lhsItem;
        }

        [[nodiscard]] auto getRhs() const -> const DotItem &
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

#endif /* CCL_PROJECT_LOGICAL_UNIT_HPP */
