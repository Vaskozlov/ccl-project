#include <ccl/lex/dot_item/logical_unit.hpp>

namespace ccl::lex::dot_item
{
    auto LogicalUnit::empty() const noexcept -> bool
    {
        return lhs_item->empty() && rhs_item->empty();
    }

    auto LogicalUnit::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        auto lhs_scan = lhs_item->scan(text_iterator);

        if (lhs_scan.has_value()) {
            return *lhs_scan;
        }

        auto rhs_scan = rhs_item->scan(text_iterator);

        if (rhs_scan.has_value()) {
            return *rhs_scan;
        }

        return 0;
    }
}// namespace ccl::lex::dot_item
