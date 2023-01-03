#ifndef CCL_PROJECT_LOGICAL_UNIT_HPP
#define CCL_PROJECT_LOGICAL_UNIT_HPP

#include <ccl/lex/dot_item/basic_item.hpp>

namespace ccl::lex::dot_item
{

    enum struct LogicalOperation : u16
    {
        NONE,
        AND,
        OR
    };

    class LogicalUnit final : public BasicItem
    {
        using typename BasicItem::TextIterator;

        DotItem rhsItem;
        DotItem lhsItem;
        LogicalOperation logicalOperation;

    public:
        [[nodiscard]] auto empty() const noexcept -> bool final;

        LogicalUnit(DotItem lhs, DotItem rhs, LogicalOperation logical_operation, Id item_id);

    private:
        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> size_t final;

        [[nodiscard]] auto orIteration(const ForkedGenerator &text_iterator) const -> size_t;
        [[nodiscard]] auto andIteration(const ForkedGenerator &text_iterator) const -> size_t;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_LOGICAL_UNIT_HPP */
