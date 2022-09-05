#ifndef CCL_PROJECT_LOGICAL_UNIT_HPP
#define CCL_PROJECT_LOGICAL_UNIT_HPP

#include <ccl/lex/dot_item/basic_item.hpp>

namespace ccl::lex::dot_item
{
    // NOLINTNEXTLINE
    CCL_ENUM(LogicalOperation, u16, NONE, AND, OR);

    class LogicalUnit final : public BasicItem
    {
    public:
        using typename BasicItem::TextIterator;

        [[nodiscard]] auto empty() const noexcept -> bool final;

        auto scanIteration(TextIterator &text_iterator, Token &token) const -> bool final;

        LogicalUnit(
            BasicItemPtr lhs_, BasicItemPtr rhs_, LogicalOperation type_,
            SpecialItems &special_items_, size_t id_, bool is_special_)
          : BasicItem(special_items_, id_), lhs_item(std::move(lhs_)), rhs_item(std::move(rhs_)),
            logical_operation(type_), is_special(is_special_)
        {}

    private:
        auto logicalAnd(TextIterator &text_iterator, Token &token) const -> bool;
        auto logicalOr(TextIterator &text_iterator, Token &token) const -> bool;

        static auto applyResult(
            std::pair<TextIterator, Token> &scan_result,
            TextIterator &text_iterator,
            Token &token) -> void;

        std::unique_ptr<BasicItem> lhs_item{};
        std::unique_ptr<BasicItem> rhs_item{};
        LogicalOperation logical_operation{ LogicalOperation::NONE };
        bool is_special{ false };
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_LOGICAL_UNIT_HPP */
