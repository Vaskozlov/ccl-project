#include <ccl/lex/dot_item/binary_expression/binary_operation_or.hpp>

namespace ccl::lexer::dot_item
{
    auto BinaryOperationOr::scanIteration(const ForkedGenerator &text_iterator) const
        -> std::optional<size_t>
    {
        const auto lhs_scan_result = lhsItem->scan(text_iterator);

        if (lhs_scan_result.has_value()) {
            return *lhs_scan_result;
        }

        return rhsItem->scan(text_iterator);
    }
}// namespace ccl::lexer::dot_item
