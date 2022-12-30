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

        UniquePtr<BasicItem> rhsItem{};
        UniquePtr<BasicItem> lhsItem{};
        LogicalOperation logicalOperation{LogicalOperation::NONE};

    public:
        [[nodiscard]] auto empty() const noexcept -> bool final;

        LogicalUnit(
            UniquePtr<BasicItem> lhs, UniquePtr<BasicItem> rhs, LogicalOperation logical_operation, Id item_id);

    private:
        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> size_t final;

        [[nodiscard]] auto orIteration(const ForkedGenerator &text_iterator) const -> size_t;
        [[nodiscard]] auto andIteration(const ForkedGenerator &text_iterator) const -> size_t;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_LOGICAL_UNIT_HPP */
