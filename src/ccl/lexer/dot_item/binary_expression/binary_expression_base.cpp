#include "ccl/lexer/dot_item/binary_expression/binary_expression_base.hpp"

namespace ccl::lexer::dot_item
{
    BinaryExpressionBase::BinaryExpressionBase(DotItem lhs, DotItem rhs, Id item_id)
      : DotItemConcept{item_id}
      , rhsItem{std::move(rhs)}
      , lhsItem{std::move(lhs)}
    {}

    auto BinaryExpressionBase::empty() const noexcept -> bool
    {
        return rhsItem->empty() && lhsItem->empty();
    }
}// namespace ccl::lexer::dot_item
