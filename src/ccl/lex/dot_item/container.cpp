#include <ccl/lex/dot_item/container.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    auto Container::beginScan(TextIterator &text_iterator, ScanningType special_scan) const
        -> std::optional<Token>
    {
        size_t totally_skipped = 0;
        auto local_iterator = text_iterator.fork();
        auto token = Token{ text_iterator, getId() };

        for (auto &&item : items) {
            auto scan_result = item->scan(local_iterator);

            if (not scan_result.has_value() && reversed) {
                scan_result = utf8::utfSize(local_iterator.getNextCarriageValue());
            }

            if (not scan_result.has_value()) {
                return std::nullopt;
            }

            if (item->hasPrefix() && *scan_result != 0) {
                token.addPrefix({ local_iterator.getRemainingAsCarriage(), *scan_result });
            } else if (item->hasPostfix() && *scan_result != 0) {
                token.addPostfix({ local_iterator.getRemainingAsCarriage(), *scan_result });
            }

            totally_skipped += *scan_result;
            local_iterator.skip(*scan_result);
        }

        if (special_scan == ScanningType::BASIC) {
            if (not(isLayoutOrEoF(local_iterator.getNextCarriageValue()) ||
                    special_items.checkForSpecial(local_iterator))) {
                return std::nullopt;
            }
        } else if (special_scan == ScanningType::CHECK) {
            return token;
        }

        token.setReprLength(totally_skipped);
        text_iterator.skip(totally_skipped);
        return token;
    }

    auto Container::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        auto local_iterator = text_iterator;
        size_t totally_skipped = 0;

        for (auto &&item : items) {
            auto scan_result = item->scan(local_iterator);

            if (not scan_result.has_value()) {
                return 0;
            }

            totally_skipped += *scan_result;
            local_iterator.skip(*scan_result);
        }

        return totally_skipped;
    }

    auto Container::parseRule(TextIterator &rule_iterator) -> void
    {
        RuleParser{ *this, rule_iterator };
    }
}// namespace ccl::lex::dot_item
