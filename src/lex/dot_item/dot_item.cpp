#include <cerberus/lex/dot_item/dot_item.hpp>

namespace cerb::lex::dot_item
{
    using namespace cerb::string_view_literals;

    auto DotItem::scanIteration(TextIterator &text_iterator) const -> bool
    {
        for (const auto &item : items) {
            if (auto [success, iterator] = item->scan(text_iterator); success) {
                text_iterator = std::move(iterator);
            } else {
                return false;
            }
        }

        return true;
    }

    auto DotItem::parseRule(TextIterator &rule_iterator) -> void
    {
        auto counter = ItemsCounter{ rule_iterator };
        rule_iterator.skipCommentsAndLayout();

        while (movedToTheNextChar(rule_iterator)) {
            auto chr = rule_iterator.getCurrentChar();
            recognizeAction(rule_iterator, chr, counter);
            rule_iterator.skipCommentsAndLayout();
        }
    }

    auto DotItem::recognizeAction(TextIterator &rule_iterator, char32_t chr, ItemsCounter &counter)
        -> void
    {
        switch (chr) {
        case U'[':
            counter += item::Union;
            emplaceItem(constructNewUnion(rule_iterator));
            break;

        case U'\"':
            counter += item::Sequence;
            emplaceItem(constructNewSequence(rule_iterator));
            break;

        case U'(':
            counter += item::DotItem;
            emplaceItem(constructNewItem(rule_iterator));
            break;

        case U'\'':
            counter += item::Terminal;
            constructTerminal(rule_iterator);
            break;

        case U'*':
            addRepetition(rule_iterator, Repetition::star());
            break;

        case U'+':
            addRepetition(rule_iterator, Repetition::plus());
            break;

        case U'?':
            addRepetition(rule_iterator, Repetition::question());
            break;

        case U'{':
            addRepetition(rule_iterator, Repetition{ rule_iterator });
            break;

        case U'^':
            reverseLastItem(rule_iterator);
            break;

        case U'c':
            counter += item::Character;
            constructString(rule_iterator, true, false);
            break;

        case U's':
            counter += item::String;
            constructString(rule_iterator, false, false);
            break;

        case U'm':
            counter += item::String;
            constructString(rule_iterator, false, true);
            break;

        default:
            throwUndefinedAction(rule_iterator);
            break;
        }
    }

    auto DotItem::constructNewSequence(TextIterator &rule_iterator) -> std::unique_ptr<BasicItem>
    {
        return std::make_unique<Sequence>(false, u8"\"", rule_iterator, analysis_shared);
    }

    auto DotItem::constructNewUnion(TextIterator &rule_iterator) -> std::unique_ptr<BasicItem>
    {
        return std::make_unique<Union>(rule_iterator, analysis_shared);
    }

    auto DotItem::constructNewItem(TextIterator &rule_iterator) -> std::unique_ptr<BasicItem>
    {
        auto text = rule_iterator.getRemaining();
        const auto *saved_end = text.end();
        auto bracket_index = text.openCloseFind('(', ')');

        if (bracket_index == u8string_view::npos) {
            throwUnterminatedDotItem(rule_iterator);
        }

        rule_iterator.setEnd(text.begin() + bracket_index);

        auto new_dot_item = std::make_unique<DotItem>(rule_iterator, id, analysis_shared);
        rule_iterator.setEnd(saved_end);

        return new_dot_item;
    }

    auto DotItem::emplaceItem(std::unique_ptr<BasicItem> item) -> void
    {
        if (not canBeOptimized()) {
            items.emplace_back(std::move(item));
        }
    }

    auto DotItem::constructString(TextIterator &rule_iterator, bool is_character, bool is_multiline)
        -> void
    {
        if (items.empty() || dynamic_cast<Sequence *>(items.back().get()) == nullptr) {
            throwUnableToApply(
                rule_iterator,
                u8"unable to apply char/string modifier, because there are not "
                "any items or the last item is not a sequence",
                u8"create sequence or do not apply string modifier to other items");
        }

        checkSize(
            rule_iterator, 1, u8"dot item with string must contain only one item - sequence",
            u8"delete other items");

        auto *sequence = dynamic_cast<Sequence *>(items.back().get());
        auto &strings_and_chars = analysis_shared.strings_and_chars;

        strings_and_chars.emplace_back(
            std::move(sequence->getRef()), 0, is_character, is_multiline);

        items.pop_back();
    }

    auto DotItem::constructTerminal(TextIterator &rule_iterator) -> void
    {
        checkSize(
            rule_iterator, 0, u8"dot item with terminal must be empty", u8"delete other items");

        auto &terminals = analysis_shared.terminals;
        auto sequence = Sequence{ false, u8"\'", rule_iterator, analysis_shared };

        terminals.addString(std::move(sequence.getRef()), id);
    }

    auto DotItem::addRepetition(TextIterator &rule_iterator, Repetition new_repetition) -> void
    {
        if (items.empty()) {
            throwUnableToApply(
                rule_iterator, u8"no item to apply repetition", u8"set repetition after item");
        }

        auto &last_item = items.back();

        if (last_item->getRepetition() != Repetition::basic()) {
            throwUnableToApply(
                rule_iterator, u8"item already has repetition",
                u8"do not set repetition more than once");
        }

        last_item->setRepetition(new_repetition);
    }

    auto DotItem::reverseLastItem(TextIterator &rule_iterator) -> void
    {
        if (items.empty()) {
            throwUnableToApply(
                rule_iterator, u8"no item to reverse", u8"create item before rever2ing it");
        }

        auto &last_item = items.back();

        if (last_item->isReversed()) {
            throwUnableToApply(
                rule_iterator, u8"item is already reversed", u8"do not set reverse for item twice");
        }

        last_item->reverse();
    }

    auto DotItem::checkSize(
        TextIterator &rule_iterator, size_t expected_size, u8string_view message,
        u8string_view suggestion) -> void
    {
        if (items.size() != expected_size) {
            throwUnexpectedSize(rule_iterator, message, suggestion);
        }
    }

    auto DotItem::throwUnexpectedSize(
        TextIterator &rule_iterator,
        u8string_view message,
        u8string_view suggestion) -> void
    {
        rule_iterator.template throwException<DotItemException>(message, suggestion);
        throw UnrecoverableError{ "unrecoverable error in dot item" };
    }

    auto DotItem::throwUnableToApply(
        TextIterator &rule_iterator,
        u8string_view reason,
        u8string_view suggestion) -> void
    {
        auto message = fmt::format<u8"unable to apply with reason: {}">(reason);
        auto converted_suggestion = fmt::format<u8"{}">(suggestion);

        rule_iterator.template throwException<DotItemException>(message, converted_suggestion);
        throw UnrecoverableError{ "unrecoverable error in DotItemType" };
    }

    auto DotItem::throwUnterminatedDotItem(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwException<DotItemException>(u8"unterminated dot item");
        throw UnrecoverableError{ "unrecoverable error in DotItemType" };
    }

    auto DotItem::throwUndefinedAction(TextIterator &rule_iterator) -> void
    {
        auto message = u8"undefined action"_sv;
        auto suggestion =
            u8"Use `\"` for string, `'` for terminal symbol, `[` for unions, `(` for dot "
            "items"_sv;

        rule_iterator.template throwException<DotItemException>(message, suggestion);
        throw UnrecoverableError{ "unrecoverable error in DotItemType" };
    }
}// namespace cerb::lex::dot_item
