#include "ccl/lexer/rule/binary_expression/binary_expression_base.hpp"

namespace ccl::lexer::rule
{
    BinaryExpressionBase::BinaryExpressionBase(RuleBlock lhs, RuleBlock rhs)
      : rhsItem{std::move(rhs)}
      , lhsItem{std::move(lhs)}
    {}

    auto BinaryExpressionBase::empty() const noexcept -> bool
    {
        return rhsItem->empty() && lhsItem->empty();
    }
}// namespace ccl::lexer::rule
