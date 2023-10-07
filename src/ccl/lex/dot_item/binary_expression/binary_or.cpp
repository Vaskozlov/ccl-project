#include <ccl/lex/dot_item/binary_expression/binary_or.hpp>

namespace ccl::lex::dot_item
{
    auto BinaryOr::scanIteration(const ForkedGenerator &text_iterator) const -> Optional<size_t>
    {
        const auto lhs_scan_result = lhsItem->scan(text_iterator);

        if (lhs_scan_result.has_value()) {
            return *lhs_scan_result;
        }

        return rhsItem->scan(text_iterator);
    }
}// namespace ccl::lex::dot_item
