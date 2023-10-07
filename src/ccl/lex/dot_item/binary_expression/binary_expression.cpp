#include "ccl/lex/dot_item/binary_expression/binary_expression.hpp"

namespace ccl::lex::dot_item
{
    BinaryExpression::BinaryExpression(DotItem lhs, DotItem rhs, Id item_id)
      : DotItemConcept{item_id}
      , rhsItem{std::move(rhs)}
      , lhsItem{std::move(lhs)}
    {}

    auto BinaryExpression::empty() const noexcept -> bool
    {
        return rhsItem->empty() && lhsItem->empty();
    }
}// namespace ccl::lex::dot_item
