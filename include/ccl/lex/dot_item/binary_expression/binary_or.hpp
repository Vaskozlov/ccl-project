#ifndef CCL_PROJECT_BINARY_OR_HPP
#define CCL_PROJECT_BINARY_OR_HPP

#include <ccl/lex/dot_item/binary_expression/binary_expression.hpp>

namespace ccl::lex::dot_item
{
    class BinaryOr final : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> Optional<size_t> override;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_BINARY_OR_HPP */
