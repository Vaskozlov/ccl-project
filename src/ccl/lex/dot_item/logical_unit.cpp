#include <ccl/lex/dot_item/logical_unit.hpp>

namespace ccl::lex::dot_item
{
    LogicalUnit::LogicalUnit(
        UniquePtr<BasicItem> lhs_, UniquePtr<BasicItem> rhs_, LogicalOperation type_, Id id_)
      : BasicItem{id_}, lhs_item{std::move(lhs_)}, rhs_item{std::move(rhs_)}, logical_operation{
                                                                                  type_}
    {}

    auto LogicalUnit::empty() const noexcept -> bool
    {
        return lhs_item->empty() && rhs_item->empty();
    }

    auto LogicalUnit::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        switch (logical_operation) {
        case LogicalOperation::OR:
            return orIteration(text_iterator);

        case LogicalOperation::AND:
            return andIteration(text_iterator);

        default:
            std::unreachable();
        }
    }

    auto LogicalUnit::orIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        return *lhs_item->scan(text_iterator)
                    .or_else([this, &text_iterator]() { return rhs_item->scan(text_iterator); })
                    .or_else([]() -> Optional<size_t> { return 0ZU; });
    }

    auto LogicalUnit::andIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        const auto lhs = lhs_item->scan(text_iterator);

        if (!lhs.has_value()) {
            return 0;
        }

        const auto rhs = rhs_item->scan(text_iterator);

        if (rhs.has_value() && (lhs == rhs)) {
            return *rhs;
        }

        return 0;
    }
}// namespace ccl::lex::dot_item
