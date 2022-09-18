#include <ccl/lex/dot_item/container.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    CCL_INLINE auto addPrefixOrPostfix(const BasicItem &item, Token &token, const string_view &repr)
        -> void
    {
        if (repr.empty()) {
        } else if (item.hasPrefix()) {
            token.addPrefix(repr);
        } else if (item.hasPostfix()) {
            token.addPostfix(repr);
        }
    }

    Container::Container(
        TextIterator &rule_iterator_, SpecialItems &special_items_, size_t id_, bool main_item_,
        bool is_special_)
      : BasicItem(id_), special_items(special_items_),
        flags({ .is_main = main_item_, .is_special = is_special_ })
    {
        parseRule(rule_iterator_);
    }

    Container::Container(
        TextIterator &&rule_iterator_, SpecialItems &special_items_, size_t id_, bool main_item_,
        bool is_special_)
      : BasicItem(id_), special_items(special_items_),
        flags({ .is_main = main_item_, .is_special = is_special_ })
    {
        parseRule(rule_iterator_);
    }

    Container::Container(
        const TextIterator &rule_iterator_, SpecialItems &special_items_, size_t id_,
        bool main_item_, bool is_special_)
      : BasicItem(id_), special_items(special_items_),
        flags({ .is_main = main_item_, .is_special = is_special_ })
    {
        auto rule_iterator = rule_iterator_;
        parseRule(rule_iterator);
    }

    auto Container::beginScan(
        TextIterator &text_iterator, Token &token, ScanningType special_scan) const -> bool
    {
        size_t totally_skipped = 0;
        auto local_iterator = text_iterator.fork();

        token.clear(getId());

        for (auto &&item : items) {
            auto scan_result = item->scan(local_iterator);

            if (not scan_result.has_value() && reversed) {
                scan_result = utf8::utfSize(local_iterator.getNextCarriageValue());
            }

            if (not scan_result.has_value()) {
                return false;
            }

            addPrefixOrPostfix(
                *item, token, { local_iterator.getRemainingAsCarriage(), *scan_result });

            totally_skipped += *scan_result;
            local_iterator.skip(*scan_result);
        }

        if (special_scan == ScanningType::BASIC) {
            if (failedToEndItem(local_iterator)) {
                return false;
            }
        } else if (special_scan == ScanningType::CHECK) {
            return true;
        }

        token.finishInitialization(text_iterator);
        token.setReprLength(totally_skipped);
        text_iterator.skip(totally_skipped);

        return true;
    }

    CCL_INLINE auto Container::failedToEndItem(const ForkedGenerator &text_iterator) const -> bool
    {
        return not(
            isLayoutOrEoF(text_iterator.getNextCarriageValue()) ||
            special_items.checkForSpecial(text_iterator));
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
