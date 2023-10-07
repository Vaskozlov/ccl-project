#include <ccl/lex/dot_item/container.hpp>

namespace ccl::lexer::dot_item
{
    using namespace ccl::string_view_literals;

    CCL_INLINE auto
        Container::addPrefixOrPostfix(const DotItemConcept *item, Token &token, string_view repr)
            -> void
    {
        if (repr.empty()) [[unlikely]] {
            return;
        }

        if (item->hasPrefix()) [[unlikely]] {
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
            auto chars_to_skip = item->scan(local_iterator);
            const bool has_moved = chars_to_skip.has_value();
            const bool succeed_as_reversed = !has_moved && isReversed();

            if (succeed_as_reversed) {
                chars_to_skip = utf8::size(local_iterator.getNextCarriageValue());
            } else if (!has_moved) {
                return false;
            }

            auto prefix_or_postfix_repr =
                string_view{local_iterator.getRemainingAsCarriage(), *chars_to_skip};

            addPrefixOrPostfix(item.get(), token, prefix_or_postfix_repr);

            totally_skipped += *chars_to_skip;
            local_iterator.skip(*chars_to_skip);
        }

        if (special_scan == ScanningType::BASIC && failedToEndItem(local_iterator)) [[unlikely]] {
            return false;
        } else if (special_scan == ScanningType::CHECK) {
            return true;
        }

        token.finishInitialization(text_iterator, totally_skipped);
        return true;
    }

    CCL_INLINE auto Container::failedToEndItem(const ForkedGenerator &text_iterator) const -> bool
    {
        return !(
            isLayoutOrEoF(text_iterator.getNextCarriageValue()) ||
            specialItems.checkForSpecial(text_iterator));
    }

    auto Container::scanIteration(const ForkedGenerator &text_iterator) const
        -> std::optional<size_t>
    {
        auto totally_skipped = as<size_t>(0);
        auto local_iterator = text_iterator;

        for (const DotItem &item : items) {
            auto scan_result = item->scan(local_iterator);

            if (!scan_result.has_value()) {
                return std::nullopt;
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
