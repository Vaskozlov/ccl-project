#include <ccl/lexer/dot_item/binary_expression/binary_operation_or.hpp>

namespace ccl::lexer::dot_item
{
    auto BinaryOperationOr::scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult
    {
        return lhsItem->scan(text_iterator).orElse([this, &text_iterator]() {
            return rhsItem->scan(text_iterator);
        });
    }

    auto BinaryOperationOr::parseIteration(const ForkedGenerator &text_iterator) const
        -> ParsingResult
    {
        auto lhs_result = lhsItem->parse(text_iterator);

        if (lhs_result.isFailure()) {
            return rhsItem->parse(text_iterator);
        }

        return lhs_result;
    }
}// namespace ccl::lexer::dot_item
