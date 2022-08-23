#include <cerberus/lex/dot_item/dot_item.hpp>

namespace cerb::lex::dot_item
{
    using namespace cerb::string_view_literals;

    auto DotItem::scanIteration(TextIterator &text_iterator, Token &token) const -> bool
    {
        for (auto &&item : items) {
            if (not scanItem(item.get(), text_iterator, token)) {
                return false;
            }
        }

        return true;
    }

    auto DotItem::scanItem(const BasicItem *item, TextIterator &text_iterator, Token &token) -> bool
    {
        auto scan_result = item->scan(text_iterator, token);

        if (scan_result.has_value()) {
            auto &[iterator, new_token] = *scan_result;
            token = std::move(new_token);
            text_iterator = std::move(iterator);
        }

        return scan_result.has_value();
    }

    auto DotItem::parseRule(TextIterator &rule_iterator) -> void
    {
        auto counter = ItemsCounter{ rule_iterator };
        rule_iterator.skipCommentsAndLayout();

        while (hasMovedToTheNextChar(rule_iterator)) {
            recognizeAction(rule_iterator, counter);
            rule_iterator.skipCommentsAndLayout();
        }

        postCreationCheck(rule_iterator, counter);
    }

    auto DotItem::hasMovedToTheNextChar(TextIterator &rule_iterator) -> bool
    {
        return not isEoF(rule_iterator.nextRawChar());
    }

    auto DotItem::recognizeAction(TextIterator &rule_iterator, ItemsCounter &items_counter) -> void
    {
        switch (rule_iterator.getCurrentChar()) {
        case U'[':
            emplaceItem(constructNewUnion(rule_iterator, items_counter));
            break;

        case U'\"':
            emplaceItem(constructNewSequence(rule_iterator, items_counter));
            break;

        case U'(':
            emplaceItem(constructNewItem(rule_iterator, items_counter));
            break;

        case U'\'':
            constructTerminal(rule_iterator, items_counter);
            break;

        case U'*':
            addRecurrence(rule_iterator, Recurrence::star());
            break;

        case U'+':
            addRecurrence(rule_iterator, Recurrence::plus());
            break;

        case U'?':
            addRecurrence(rule_iterator, Recurrence::question());
            break;

        case U'{':
            addRecurrence(rule_iterator, Recurrence{ rule_iterator });
            break;

        case U'^':
            reverseLastItem(rule_iterator);
            break;

        case U'p':
            checkAbilityToCreatePrefixPostfix(rule_iterator);
            addPrefixPostfix();
            break;

        case U'c':
            if (rule_iterator.futureRawChar(1) == U'o') {
                rule_iterator.nextRawChar();
                constructComment(rule_iterator);
            } else {
                constructString(items_counter, true, false);
            }

            break;

        case U's':
            constructString(items_counter, false, false);
            break;

        case U'm':
            constructString(items_counter, false, true);
            break;

        default:
            throwUndefinedAction(rule_iterator);
            break;
        }
    }

    auto DotItem::constructNewSequence(TextIterator &rule_iterator, ItemsCounter &items_counter)
        -> std::unique_ptr<BasicItem>
    {
        items_counter.add(item::Sequence);

        return std::make_unique<Sequence>(
            Sequence::SequenceFlags{}, u8"\"", rule_iterator, analysis_shared);
    }

    auto DotItem::constructNewUnion(TextIterator &rule_iterator, ItemsCounter &items_counter)
        -> std::unique_ptr<BasicItem>
    {
        items_counter.add(item::Union);

        return std::make_unique<Union>(rule_iterator, analysis_shared);
    }

    // NOLINTNEXTLINE (recursive function)
    auto DotItem::constructNewItem(TextIterator &rule_iterator, ItemsCounter &items_counter)
        -> std::unique_ptr<BasicItem>
    {
        items_counter.add(item::DotItem);

        auto text = rule_iterator.getRemaining();
        const auto *saved_end = text.end();
        auto bracket_index = findDotItemEnd(rule_iterator, text);

        rule_iterator.setEnd(text.begin() + bracket_index);

        auto new_dot_item = std::make_unique<DotItem>(rule_iterator, id, analysis_shared, false);
        rule_iterator.setEnd(saved_end);

        return new_dot_item;
    }

    auto DotItem::constructString(ItemsCounter &items_counter, bool is_character, bool is_multiline)
        -> void
    {
        if (is_character) {
            items_counter.add(item::Character);
        } else {
            items_counter.add(item::String);
        }

        auto *sequence = unsafeGetLastItemAs<Sequence>();// check is above
        auto &strings_and_chars = analysis_shared.strings_and_chars;
        auto &string = sequence->getByRef();
        auto column_index = string.find(u8':');

        if (column_index == u8string_view::npos) {
            strings_and_chars.emplace_back(string, id, is_character, is_multiline);
        } else {
            auto string_begin = string.substr(0, column_index);
            auto string_end = string.substr(column_index + 1);

            strings_and_chars.emplace_back(
                std::move(string_begin), std::move(string_end), id, is_character, is_multiline);
        }

        items.pop_back();
    }

    auto DotItem::constructTerminal(TextIterator &rule_iterator, ItemsCounter &items_counter)
        -> void
    {
        items_counter.add(item::Terminal);

        auto &terminals = analysis_shared.terminals;
        auto sequence = Sequence{ {}, u8"\'", rule_iterator, analysis_shared };

        terminals.addString(std::move(sequence.getByRef()), id);
    }

    auto DotItem::emplaceItem(std::unique_ptr<BasicItem> &&item) -> void
    {
        if (not canBeOptimized()) {
            items.emplace_back(std::move(item));
        }
    }

    auto DotItem::addPrefixPostfix() -> void
    {
        auto &last_item = items.back();
        auto is_prefix = items.size() == 1 || items[items.size() - 2]->hasPrefix();

        if (is_prefix) {
            last_item->setPrefix();
        } else {
            last_item->setPostfix();
        }
    }

    auto DotItem::constructComment(TextIterator &rule_iterator) -> void
    {
        checkThereIsOnlySequence(rule_iterator, u8"comment");

        auto *sequence = unsafeGetLastItemAs<Sequence>();
        auto &string = sequence->getByRef();
        auto column_index = string.find(u8':');
        auto &[single_line_comment, multiline_begin_comment, multiline_end_comment] =
            analysis_shared.comment_tokens;

        if (column_index == u8string_view::npos) {
            single_line_comment = string;
        } else {
            auto comment_begin = string.substr(0, column_index);
            auto comment_end = string.substr(column_index + 1);

            multiline_begin_comment = comment_begin;
            multiline_end_comment = comment_end;
        }

        items.pop_back();
    }

    auto DotItem::addRecurrence(TextIterator &rule_iterator, Recurrence new_recurrence) -> void
    {
        if (items.empty()) {
            throwUnableToApply<u8"no items found", u8"set recurrence modifier after item">(
                rule_iterator);
        }

        auto &last_item = items.back();

        if (last_item->getRecurrence() != Recurrence::basic()) {
            throwUnableToApply<
                u8"item already has recurrence", u8"do not set recurrence more than once">(
                rule_iterator);
        }

        last_item->setRecurrence(new_recurrence);
    }

    auto DotItem::reverseLastItem(TextIterator &rule_iterator) -> void
    {
        if (items.empty()) {
            throwUnableToApply<u8"no items to reverse">(rule_iterator);
        }

        auto &last_item = items.back();

        if (last_item->isReversed()) {
            throwUnableToApply<
                u8"item already has reverse modifier", u8"do not reverse it more than once">(
                rule_iterator);
        }

        last_item->reverse();
    }

    auto DotItem::postCreationCheck(TextIterator &rule_iterator, const ItemsCounter &counter)
        -> void
    {
        if (counter.hasStrOrChar() && counter.hasSequences()) {
            throwUnableToApply<
                u8"string or character expected, but got sequence",
                u8"add string or character modifier to the sequence">(rule_iterator);
        }

        auto postfix_elem =
            std::ranges::find_if(items, [](const auto &elem) { return elem->hasPostfix(); });

        auto are_postfixes_correct = std::all_of(
            postfix_elem, items.end(), [](const auto &elem) { return elem->hasPostfix(); });

        if (not are_postfixes_correct) {
            auto suggestion = fmt::format<u8"add postfix modifier to the last item\n{}p">(
                rule_iterator.getWorkingLine());
            throwUnableToApply(
                rule_iterator, u8"item without postfix modifier exists after items with it"_sv,
                suggestion);
        }
    }

    auto DotItem::findDotItemEnd(TextIterator &rule_iterator, u8string_view repr) -> size_t
    {
        auto bracket_index = repr.openCloseFind(u8'(', u8')');

        if (not bracket_index.has_value()) {
            rule_iterator.throwException<DotItemException>(u8"unterminated dot item");
            throw UnrecoverableError{ "unrecoverable error in DotItemType" };
        }

        return *bracket_index;
    }

    // TODO: replace it with items counter checks
    auto DotItem::checkThereIsOnlySequence(TextIterator &rule_iterator, u8string_view modifier)
        -> void
    {
        if (items.empty() || dynamic_cast<Sequence *>(items.back().get()) == nullptr) {
            auto message = fmt::format<u8"not sequence found to apply {} modifier">(modifier);

            auto suggestion = fmt::format<
                u8"create sequence or do not apply {} modifier to "
                "other items">(modifier);

            throwUnableToApply(rule_iterator, message, suggestion);
        }

        if (items.size() != 1) {
            auto message =
                fmt::format<u8"{} modifier, the number of items is more than 1">(modifier);
            throwUnableToApply(rule_iterator, message);
        }
    }

    auto DotItem::checkAbilityToCreatePrefixPostfix(TextIterator &rule_iterator) -> void
    {
        if (not main_item) {
            throwUnableToApply<
                u8"you are not allowed to create prefixes or postfixes inside other dot items",
                u8"create them only in main item">(rule_iterator);
        }

        if (items.empty()) {
            throwUnableToApply<u8"there are not any items to apply prefix/postfix">(rule_iterator);
        }

        auto &last_item = items.back();

        if (last_item->hasPrefix()) {
            throwUnableToApply<
                u8"item already has prefix modifier", u8"do not add it more than once">(
                rule_iterator);
        }

        if (last_item->hasPostfix()) {
            throwUnableToApply<
                u8"item already has postfix modifier", u8"do not add it more than once">(
                rule_iterator);
        }
    }

    template<ConstString Reason, ConstString Suggestion>
    auto DotItem::throwUnableToApply(TextIterator &rule_iterator) -> void
    {
        static constexpr auto reason = fmt::staticFormat<u8"unable to apply: {}", Reason>();

        rule_iterator.throwException<DotItemException>(reason, Suggestion);
        throw UnrecoverableError{ "unrecoverable error in DotItemType" };
    }

    auto DotItem::throwUnableToApply(
        TextIterator &rule_iterator,
        u8string_view reason,
        u8string_view suggestion) -> void
    {
        auto formatted_reason = fmt::format<u8"unable to apply: {}">(reason);

        rule_iterator.throwException<DotItemException>(formatted_reason, suggestion);
        throw UnrecoverableError{ "unrecoverable error in DotItemType" };
    }

    auto DotItem::throwUndefinedAction(TextIterator &rule_iterator) -> void
    {
        auto message = u8"undefined action"_sv;
        auto suggestion =
            u8"Use `\"` for string, `'` for terminal symbol, `[` for unions, `(` for dot "
            "items"_sv;

        rule_iterator.throwException<DotItemException>(message, suggestion);
        throw UnrecoverableError{ "unrecoverable error in DotItemType" };
    }
}// namespace cerb::lex::dot_item
