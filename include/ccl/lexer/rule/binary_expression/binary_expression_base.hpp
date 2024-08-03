#ifndef CCL_PROJECT_BINARY_EXPRESSION_BASE_HPP
#define CCL_PROJECT_BINARY_EXPRESSION_BASE_HPP

#include <ccl/lexer/rule/rule_block_interface.hpp>

namespace ccl::lexer::rule
{
    class BinaryOperationOr;
    class BinaryOperationAnd;

    enum struct BinaryOperator : u16
    {
        NONE,
        AND,
        OR
    };

    class BinaryExpressionBase : public RuleBlockInterface
    {
    private:
        friend BinaryOperationOr;
        friend BinaryOperationAnd;
        using typename RuleBlockInterface::TextIterator;

        RuleBlock rhsItem;
        RuleBlock lhsItem;

    public:
        [[nodiscard]] BinaryExpressionBase(RuleBlock lhs, RuleBlock rhs, SmallId item_id);

        [[nodiscard]] auto empty() const noexcept -> bool override;

        [[nodiscard]] auto getLhs() const CCL_LIFETIMEBOUND -> const RuleBlock &
        {
            return lhsItem;
        }

        [[nodiscard]] auto getRhs() const CCL_LIFETIMEBOUND -> const RuleBlock &
        {
            return rhsItem;
        }
    };
}// namespace ccl::lexer::rule

#endif /* CCL_PROJECT_BINARY_EXPRESSION_BASE_HPP */
