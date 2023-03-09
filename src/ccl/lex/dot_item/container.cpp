#include <ccl/lex/dot_item/container.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    CCL_INLINE auto Container::addPrefixOrPostfix(
        const DotItemConcept *item, Token &token, const string_view &repr) -> void
    {
        if (repr.empty()) [[unlikely]] {
        } else if (item->hasPrefix()) [[unlikely]] {
            token.addPrefix(repr);
        } else if (item->hasPostfix()) [[unlikely]] {
            token.addPostfix(repr);
        }
    }

    Container::Container(
        TextIterator &rule_iterator, SpecialItems &special_items, Id item_id, bool main_item,
        bool is_special)
      : DotItemConcept{item_id}
      , specialItems{special_items}
      , flags{.isMain = main_item, .isSpecial = is_special}
    {
        parseRule(rule_iterator);
    }

    Container::Container(
        const TextIterator &rule_iterator, SpecialItems &special_items, Id item_id, bool main_item,
        bool is_special)
      : DotItemConcept{item_id}
      , specialItems{special_items}
      , flags{.isMain = main_item, .isSpecial = is_special}
    {
        auto text_iterator_copy = rule_iterator;
        parseRule(text_iterator_copy);
    }

    auto Container::beginScan(
        TextIterator &text_iterator, Token &token, ScanningType special_scan) const -> bool
    {
        auto totally_skipped = as<size_t>(0);
        auto local_iterator = text_iterator.fork();

        token.clear(getId());

        for (const DotItem &item : items) {
            auto char_to_skip = item->scan(local_iterator);

            if ((!char_to_skip.has_value()) && isReversed()) {
                char_to_skip = utf8::size(local_iterator.getNextCarriageValue());
            }

            if (!char_to_skip.has_value()) {
                return false;
            }

            addPrefixOrPostfix(
                item.get(), token, {local_iterator.getRemainingAsCarriage(), *char_to_skip});

            totally_skipped += *char_to_skip;
            local_iterator.skip(*char_to_skip);
        }

        if (ScanningType::BASIC == special_scan) {
            if (failedToEndItem(local_iterator)) [[unlikely]] {
                return false;
            }
        } else if (ScanningType::CHECK == special_scan) {
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
            specialItems.checkForSpecial(text_iterator));
    }

    auto Container::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        auto totally_skipped = as<size_t>(0);
        auto local_iterator = text_iterator;

        for (auto &&item : items) {
            auto scan_result = item->scan(local_iterator);

            if (!scan_result.has_value()) {
                return 0;
            }

            totally_skipped += *scan_result;
            local_iterator.skip(*scan_result);
        }

        return totally_skipped;
    }

    auto Container::parseRule(TextIterator &rule_iterator) -> void
    {
        RuleParser{*this, rule_iterator};
    }
}// namespace ccl::lex::dot_item
