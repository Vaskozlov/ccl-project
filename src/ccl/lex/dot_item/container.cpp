#include <ccl/lex/dot_item/container.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    auto Container::scanIteration(TextIterator &text_iterator, Token &token) const -> bool
    {
        for (auto &&item : items) {
            if (not scanItem(item.get(), text_iterator, token)) {
                return false;
            }
        }

        return true;
    }

    auto Container::scanItem(const BasicItem *item, TextIterator &text_iterator, Token &token) const
        -> bool
    {
        auto scan_type = is_special ? ScanType::SPECIAL : ScanType::FORKED;
        auto scan_result = item->scan(text_iterator, token, scan_type);

        if (scan_result.has_value()) {
            auto &[iterator, new_token] = *scan_result;
            token = std::move(new_token);
            text_iterator = std::move(iterator);
        }

        return scan_result.has_value();
    }

    // NOLINTNEXTLINE recursive function
    auto Container::parseRule(TextIterator &rule_iterator) -> void
    {
        RuleParser{ *this, rule_iterator };
    }
}// namespace ccl::lex::dot_item
