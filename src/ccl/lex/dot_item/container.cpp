#include <ccl/lex/dot_item/container.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    const static Token EmptyToken{};

    auto Container::scanIteration(TextIterator &text_iterator) const -> bool
    {
        for (auto &&item : items) {
            if (not scanItem(item.get(), text_iterator)) {
                return false;
            }
        }

        return true;
    }

    auto Container::scanItem(const BasicItem *item, TextIterator &text_iterator) const -> bool
    {
        ScanType scan_type = is_special ? ScanType::SPECIAL : ScanType::FORKED;
        auto scan_result = item->scan(text_iterator, EmptyToken, scan_type);

        if (scan_result.has_value()) {
            auto &[iterator, new_token] = *scan_result;
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
