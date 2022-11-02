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
      : BasicItem(id_), specialItems(special_items_),
        flags({ .is_main = main_item_, .is_special = is_special_ })
    {
        parseRule(rule_iterator_);
    }

    Container::Container(
        TextIterator &&rule_iterator_, SpecialItems &special_items_, size_t id_, bool main_item_,
        bool is_special_)
      : BasicItem(id_), specialItems(special_items_),
        flags({ .is_main = main_item_, .is_special = is_special_ })
    {
        parseRule(rule_iterator_);
    }

    Container::Container(
        const TextIterator &rule_iterator_, SpecialItems &special_items_, size_t id_,
        bool main_item_, bool is_special_)
      : BasicItem(id_), specialItems(special_items_),
        flags({ .is_main = main_item_, .is_special = is_special_ })
    {
        auto rule_iterator = rule_iterator_;
        parseRule(rule_iterator);
    }

    auto Container::beginScan(
        TextIterator &text_iterator, Token &token, ScanningType special_scan) const -> bool
    {
        auto totally_skipped = 0ZU;
        auto local_iterator = text_iterator.fork();

        token.clear(getId());

        for (auto &&item : items) {
            auto char_to_skip = item->scan(local_iterator);

            if (not char_to_skip.has_value() && isReversed()) {
                char_to_skip = utf8::size(local_iterator.getNextCarriageValue());
            }

            if (not char_to_skip.has_value()) {
                return false;
            }

            addPrefixOrPostfix(
                *item, token, { local_iterator.getRemainingAsCarriage(), *char_to_skip });

            totally_skipped += *char_to_skip;
            local_iterator.skip(*char_to_skip);
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
            specialItems.checkForSpecial(text_iterator));
    }

    auto Container::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        auto totally_skipped = 0ZU;
        auto local_iterator = text_iterator;

        for (auto &&item : items) {
            auto scan_result = item->scan(local_iterator);

            if (not scan_result.has_value()) {
                return 0ZU;
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
