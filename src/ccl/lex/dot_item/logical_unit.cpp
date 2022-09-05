#include <ccl/lex/dot_item/logical_unit.hpp>

namespace ccl::lex::dot_item
{
    auto LogicalUnit::empty() const noexcept -> bool
    {
        return lhs_item->empty() && rhs_item->empty();
    }

    auto LogicalUnit::scanIteration(TextIterator &text_iterator, Token &token) const -> bool
    {
        switch (logical_operation) {
        case LogicalOperation::AND:
            return logicalAnd(text_iterator, token);

        case LogicalOperation::OR:
            return logicalOr(text_iterator, token);

        default:
            throw InvalidArgument{ "undefined action" };
        }
    }

    auto LogicalUnit::logicalAnd(TextIterator &text_iterator, Token &token) const -> bool
    {
        ScanType scan_type = is_special ? ScanType::SPECIAL : ScanType::FORKED;

        auto lhs_scan_result = lhs_item->scan(text_iterator, token, scan_type);
        auto rhs_scan_result = rhs_item->scan(text_iterator, token, scan_type);

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
        ScanType scan_type = is_special ? ScanType::SPECIAL : ScanType::FORKED;

        auto lhs_scan_result = lhs_item->scan(text_iterator, token, scan_type);
        auto rhs_scan_result = rhs_item->scan(text_iterator, token, scan_type);

        auto has_lhs = lhs_scan_result.has_value();
        auto has_rhs = rhs_scan_result.has_value();

        if (not lor(has_lhs, has_rhs)) {
            return false;
        }

        if (land(has_lhs, not has_rhs)) {
            applyResult(*lhs_scan_result, text_iterator, token);
            return true;
        }

        if (land(not has_lhs, has_rhs)) {
            applyResult(*rhs_scan_result, text_iterator, token);
            return true;
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
