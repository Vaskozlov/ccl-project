#include <ccl/lex/dot_item/container.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    static auto splitStringByIndex(const std::string &string, size_t index)
        -> std::pair<std::string, std::string>
    {
        return { string.substr(0, index), string.substr(index + 1) };
    }

    auto Container::scanIteration(TextIterator &text_iterator, Token &token) const -> bool
    {
        for (auto &&item : items) {
            if (not scanItem(item.get(), text_iterator, token)) {
                return false;
            }
        }

        return true;
    }

    auto Container::scanItem(const BasicItem *item, TextIterator &text_iterator, Token &token)
        -> bool
    {
        auto scan_result = item->scan(text_iterator, token);

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
        checkId();

        auto counter = ItemsCounter{ rule_iterator };
        rule_iterator.skipCommentsAndLayout();

        while (hasMovedToTheNextChar(rule_iterator)) {
            recognizeAction(rule_iterator, counter);
            rule_iterator.skipCommentsAndLayout();
        }

        postCreationCheck(rule_iterator, counter);
    }

    auto Container::hasMovedToTheNextChar(TextIterator &rule_iterator) -> bool
    {
        return not isEoF(rule_iterator.next());
    }

    // NOLINTNEXTLINE recursive function
    auto Container::recognizeAction(TextIterator &rule_iterator, ItemsCounter &items_counter)
        -> void
    {
        switch (rule_iterator.getCurrentChar()) {
        case U'[':
            emplaceItem(rule_iterator, constructNewUnion(rule_iterator, items_counter));
            break;

        case U'\"':
            emplaceItem(rule_iterator, constructNewSequence(rule_iterator, items_counter));
            break;

        case U'(':
            emplaceItem(rule_iterator, constructNewItem(rule_iterator, items_counter));
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
            constructCommentOrCharacter(rule_iterator, items_counter);
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

    auto Container::constructNewSequence(TextIterator &rule_iterator, ItemsCounter &items_counter)
        -> std::unique_ptr<BasicItem>
    {
        items_counter.add(item::Sequence);

        return std::make_unique<Sequence>(
            Sequence::SequenceFlags{}, "\"", rule_iterator, analysis_shared);
    }

    auto Container::constructNewUnion(TextIterator &rule_iterator, ItemsCounter &items_counter)
        -> std::unique_ptr<BasicItem>
    {
        items_counter.add(item::Union);

        return std::make_unique<Union>(rule_iterator, analysis_shared);
    }

    // NOLINTNEXTLINE (recursive function)
    auto Container::constructNewItem(TextIterator &rule_iterator, ItemsCounter &items_counter)
        -> std::unique_ptr<BasicItem>
    {
        items_counter.add(item::Container);

        auto text = rule_iterator.getRemaining();
        const auto *saved_end = rule_iterator.getEnd();
        auto bracket_index = findContainerEnd(rule_iterator, text);

        rule_iterator.setEnd(text.begin() + bracket_index);

        auto new_container = std::make_unique<Container>(rule_iterator, id, analysis_shared, false);
        rule_iterator.setEnd(saved_end);

        return new_container;
    }

    auto Container::constructString(
        ItemsCounter &items_counter,
        bool is_character,
        bool is_multiline) -> void
    {
        if (is_character) {
            items_counter.add(item::Character);
        } else {
            items_counter.add(item::String);
        }

        auto *sequence = unsafeGetLastItemAs<Sequence>();// check is above
        auto &strings_and_chars = analysis_shared.strings_and_chars;
        auto &string = sequence->getByRef();
        auto column_index = string.find(':');

        if (column_index == std::string::npos) {
            strings_and_chars.emplace_back(std::move(string), id, is_character, is_multiline);
        } else {
            auto [string_begin, string_end] = splitStringByIndex(string, column_index);

            strings_and_chars.emplace_back(
                std::move(string_begin), std::move(string_end), id, is_character, is_multiline);
        }

        items.pop_back();
    }

    auto Container::constructTerminal(TextIterator &rule_iterator, ItemsCounter &items_counter)
        -> void
    {
        items_counter.add(item::Terminal);

        auto &special_tokens = analysis_shared.special_tokens;
        auto sequence = Sequence{ {}, "\'", rule_iterator, analysis_shared };

        special_tokens.addString(std::move(sequence.getByRef()), id);
    }

    auto Container::constructComment(ItemsCounter &items_counter) -> void
    {
        items_counter.add(item::Comment);

        auto *sequence = unsafeGetLastItemAs<Sequence>();// check is above
        auto &string = sequence->getByRef();
        auto column_index = string.find(':');
        auto &[single_line_comment, multiline_begin_comment, multiline_end_comment] =
            analysis_shared.comment_tokens;

        if (column_index == std::string::npos) {
            single_line_comment = string;
        } else {
            auto [comment_begin, comment_end] = splitStringByIndex(string, column_index);
            multiline_begin_comment = std::move(comment_begin);
            multiline_end_comment = std::move(comment_end);
        }

        items.pop_back();
    }

    auto Container::constructCommentOrCharacter(
        TextIterator &rule_iterator,
        ItemsCounter &items_counter) -> void
    {
        if (rule_iterator.isNextCharacterEqual<U'o'>()) {
            rule_iterator.next();
            constructComment(items_counter);
        } else {
            constructString(items_counter, true, false);
        }
    }

    auto Container::emplaceItem(TextIterator &rule_iterator, std::unique_ptr<BasicItem> &&item)
        -> void
    {
        if (not canBeOptimized()) {
            auto item_recurrence = item->getRecurrence();

            BasicItem::neverRecognizedSuggestion(
                rule_iterator, item_recurrence.from == 0 && not reversed && not item->empty());

            BasicItem::alwaysRecognizedSuggestion(rule_iterator, not reversed && item->empty());

            items.emplace_back(std::move(item));
        }
    }

    auto Container::addPrefixPostfix() -> void
    {
        auto &last_item = items.back();
        auto is_prefix = items.size() == 1 || items[items.size() - 2]->hasPrefix();

        if (is_prefix) {
            last_item->setPrefix();
        } else {
            last_item->setPostfix();
        }
    }

    auto Container::addRecurrence(TextIterator &rule_iterator, Recurrence new_recurrence) -> void
    {
        if (items.empty()) {
            throwUnableToApply(
                rule_iterator, "no items found", "set recurrence modifier after item");
            return;
        }

        auto &last_item = items.back();

        if (last_item->getRecurrence() != Recurrence::basic()) {
            throwUnableToApply(
                rule_iterator,
                "item already has recurrence",
                "do not set recurrence more than once");
            return;
        }

        last_item->setRecurrence(new_recurrence);
    }

    auto Container::reverseLastItem(TextIterator &rule_iterator) -> void
    {
        if (items.empty()) {
            throwUnableToApply(rule_iterator, "no items to reverse");
            return;
        }

        auto &last_item = items.back();

        if (last_item->isReversed()) {
            throwUnableToApply(
                rule_iterator,
                "item already has reverse modifier",
                "do not reverse it more than once");
            return;
        }

        last_item->reverse();
    }

    auto Container::postCreationCheck(TextIterator &rule_iterator, const ItemsCounter &counter)
        -> void
    {
        if (counter.hasStrOrChar() && counter.hasSequences()) {
            throwUnableToApply(
                rule_iterator,
                "string or character expected, but got sequence",
                "add string or character modifier to the sequence");
            return;
        }

        auto postfix_elem =
            std::ranges::find_if(items, [](const auto &elem) { return elem->hasPostfix(); });

        auto are_postfixes_correct = std::all_of(
            postfix_elem, items.end(), [](const auto &elem) { return elem->hasPostfix(); });

        if (not are_postfixes_correct) {
            auto suggestion = fmt::format(
                "add postfix modifier to the last item\n{}p", rule_iterator.getWorkingLine());

            throwUnableToApply(
                rule_iterator,
                "item without postfix modifier exists after items with it",
                suggestion);
            return;
        }

        BasicItem::neverRecognizedSuggestion(rule_iterator, not counter.hasAny() && not reversed);

        BasicItem::alwaysRecognizedSuggestion(rule_iterator, not counter.hasAny() && reversed);
    }

    auto Container::findContainerEnd(TextIterator &rule_iterator, string_view repr) -> size_t
    {
        auto bracket_index = repr.openCloseFind('(', ')');

        if (not bracket_index.has_value()) {
            rule_iterator.throwPanicError("unterminated dot item");
            throw UnrecoverableError{ "unrecoverable error in ContainerType" };
        }

        return *bracket_index;
    }

    auto Container::checkId() const -> void
    {
        if (ReservedTokenType::contains(id)) {
            throw UnrecoverableError{
                "reserved token type (0 and 1 are reserved for EOI and BAD TOKEN)"
            };
        }
    }

    auto Container::checkAbilityToCreatePrefixPostfix(TextIterator &rule_iterator) -> void
    {
        if (not main_item) {
            throwUnableToApply(
                rule_iterator,
                "you are not allowed to create prefixes or postfixes inside other dot items",
                "create them only in main item");
            return;
        }

        if (items.empty()) {
            throwUnableToApply(rule_iterator, "there are not any items to apply prefix/postfix");
            return;
        }

        auto &last_item = items.back();

        if (last_item->hasPrefix()) {
            throwUnableToApply(
                rule_iterator, "item already has prefix modifier", "do not add it more than once");
            return;
        }

        if (last_item->hasPostfix()) {
            throwUnableToApply(
                rule_iterator, "item already has postfix modifier", "do not add it more than once");
            return;
        }
    }

    auto Container::throwUnableToApply(
        TextIterator &rule_iterator,
        string_view reason,
        string_view suggestion) -> void
    {
        using namespace fmt::literals;

        auto message = fmt::format("unable to apply: {}", reason);

        rule_iterator.throwCriticalError(message, suggestion);
        throw UnrecoverableError{ "unrecoverable error in ContainerType" };
    }

    auto Container::throwUndefinedAction(TextIterator &rule_iterator) -> void
    {
        auto message = "undefined action"_sv;
        auto suggestion =
            "Use `\"` for string, `'` for special symbol, `[` for unions, `(` for dot "
            "items"_sv;

        rule_iterator.throwPanicError(message, suggestion);
        throw UnrecoverableError{ "unrecoverable error in ContainerType" };
    }
}// namespace ccl::lex::dot_item