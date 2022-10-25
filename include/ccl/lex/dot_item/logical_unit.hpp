#ifndef CCL_PROJECT_LOGICAL_UNIT_HPP
#define CCL_PROJECT_LOGICAL_UNIT_HPP

#include <ccl/lex/dot_item/basic_item.hpp>

namespace ccl::lex::dot_item
{
    // NOLINTNEXTLINE
    CCL_ENUM(LogicalOperation, u16, NONE, AND, OR);

    class LogicalUnit final : public BasicItem
    {
        using typename BasicItem::TextIterator;

        UniquePtr<BasicItem> lhs_item{};
        UniquePtr<BasicItem> rhs_item{};
        LogicalOperation logical_operation{ LogicalOperation::NONE };

    public:
        [[nodiscard]] auto empty() const noexcept -> bool final;

        LogicalUnit(
            UniquePtr<BasicItem> lhs_, UniquePtr<BasicItem> rhs_, LogicalOperation type_,
            size_t id_)
          : BasicItem(id_), lhs_item(std::move(lhs_)), rhs_item(std::move(rhs_)),
            logical_operation(type_)
        {}

    private:
        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> size_t final;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_LOGICAL_UNIT_HPP */
