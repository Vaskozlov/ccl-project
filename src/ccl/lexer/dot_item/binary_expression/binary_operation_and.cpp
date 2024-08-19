#include <ccl/lexer/rule/binary_expression/binary_operation_and.hpp>

namespace ccl::lexer::rule
{
    auto BinaryOperationAnd::scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult
    {
        const auto lhs = rhsItem->scan(text_iterator);

        if (lhs.isFailure()) {
            return ScanResult::failure();
        }

        const auto rhs = lhsItem->scan(text_iterator);
        return lhs == rhs ? lhs : ScanResult::failure();
    }
}// namespace ccl::lexer::rule
