#include <ccl/lex/dot_item/logical_unit.hpp>

namespace ccl::lex::dot_item
{
    auto LogicalUnit::empty() const noexcept -> bool
    {
        return lhs_item->empty() && rhs_item->empty();
    }

    auto LogicalUnit::scanIteration(TextIterator &text_iterator, Token &token) const -> bool
    {
        switch (logical_unit_type) {
        case LogicalUnitType::AND:
            return logicalAnd(text_iterator, token);

        case LogicalUnitType::OR:
            return logicalOr(text_iterator, token);

        default:
            throw InvalidArgument{ "undefined action" };
        }
    }

    auto LogicalUnit::logicalAnd(TextIterator &text_iterator, Token &token) const -> bool
    {
        auto lhs_scan_result = lhs_item->scan(text_iterator, token);
        auto rhs_scan_result = rhs_item->scan(text_iterator, token);

        if (not lhs_scan_result.has_value() || not rhs_scan_result.has_value()) {
            return false;
        }

        auto &[lhs_iterator, lhs_token] = *lhs_scan_result;
        auto &[rhs_iterator, rhs_token] = *rhs_scan_result;

        auto lhs_repr = lhs_token.getRepr();
        auto rhs_repr = rhs_token.getRepr();

        if (lhs_repr.size() != rhs_repr.size()) {
            return false;
        }

        applyResult(*lhs_scan_result, text_iterator, token);

        return true;
    }

    auto LogicalUnit::logicalOr(TextIterator &text_iterator, Token &token) const -> bool
    {
        auto lhs_scan_result = lhs_item->scan(text_iterator, token);
        auto rhs_scan_result = rhs_item->scan(text_iterator, token);

        if (not lhs_scan_result.has_value() && not rhs_scan_result.has_value()) {
            return false;
        }

        if (lhs_scan_result.has_value() && not rhs_scan_result.has_value()) {
            applyResult(*lhs_scan_result, text_iterator, token);
            return true;
        }

        if (not lhs_scan_result.has_value() && rhs_scan_result.has_value()) {
            applyResult(*rhs_scan_result, text_iterator, token);
        }

        auto &[lhs_iterator, lhs_token] = *lhs_scan_result;
        auto &[rhs_iterator, rhs_token] = *rhs_scan_result;

        auto lhs_repr = lhs_token.getRepr();
        auto rhs_repr = rhs_token.getRepr();

        if (lhs_repr.size() > rhs_repr.size()) {
            applyResult(*lhs_scan_result, text_iterator, token);
        } else {
            applyResult(*rhs_scan_result, text_iterator, token);
        }

        return true;
    }

    auto LogicalUnit::applyResult(
        std::pair<TextIterator, Token> &scan_result, TextIterator &text_iterator, Token &token)
        -> void
    {
        auto &[iterator, new_token] = scan_result;

        token = std::move(new_token);
        text_iterator = std::move(iterator);
    }
}// namespace ccl::lex::dot_item
