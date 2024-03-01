export module ccl.lexer.dot_item.binary_expression:logical_and;

export import ccl.text;
export import :base_class;

export namespace ccl::lexer::dot_item
{
    class BinaryOperationAnd final : public BinaryExpressionBase
    {
    public:
        using BinaryExpressionBase::BinaryExpressionBase;

        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> std::optional<std::size_t> override
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
    };
}// namespace ccl::lexer::dot_item
