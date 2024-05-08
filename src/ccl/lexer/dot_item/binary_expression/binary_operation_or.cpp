#include <ccl/lexer/dot_item/binary_expression/binary_operation_or.hpp>

namespace ccl::lexer::dot_item
{
    auto BinaryOperationOr::scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult
    {
        return lhsItem->scan(text_iterator).orElse([this, &text_iterator]() {
            return rhsItem->scan(text_iterator);
        });
    }
}// namespace ccl::lexer::dot_item
