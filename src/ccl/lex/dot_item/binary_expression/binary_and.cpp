#include <ccl/lex/dot_item/binary_expression/binary_and.hpp>

namespace ccl::lex::dot_item
{
    auto BinaryAnd::scanIteration(const ForkedGenerator &text_iterator) const -> Optional<size_t>
    {
        const auto lhs = rhsItem->scan(text_iterator);

        if (!lhs.has_value()) {
            return std::nullopt;
        }

        const auto rhs = lhsItem->scan(text_iterator);

        if (rhs.has_value() && (lhs == rhs)) {
            return *lhs;
        }

        return std::nullopt;
    }
}// namespace ccl::lex::dot_item
