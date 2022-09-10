#include <ccl/lex/dot_item/sequence.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    Sequence::Sequence(
        SequenceFlags flags_, const string_view &str_begin_, const string_view &str_end_,
        TextIterator &rule_iterator_, SpecialItems &special_items_, size_t id_)
      : BasicItem(special_items_, id_), str_begin(str_begin_), str_end(str_end_),
        sequence_flags(flags_)
    {
        auto &rule_iterator = rule_iterator_;
        auto begin_iterator_state = rule_iterator;

        checkSequenceArguments(rule_iterator);
        skipStringDefinition(rule_iterator);

        while (true) {
            auto chr = U'\0';
            auto is_escaping = false;

            if (sequence_flags.no_escaping_symbols) {
                chr = rule_iterator.next();
            } else {
                auto [escaping, character] = rule_iterator.nextRawCharWithEscapingSymbols();
                is_escaping = escaping;
                chr = character;
            }

            checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

            if (isStringEnd(rule_iterator, is_escaping)) {
                rule_iterator.skip(str_end.size() - 1);
                break;
            }

            utf8::appendUtf32ToUtf8Container(sequence_value, chr);
        }
    }

    auto Sequence::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        auto future_text = text_iterator.getFutureRemaining<std::string_view>();

        if (future_text.starts_with(sequence_value) ^ reversed) {
            return reversed ? utf8::utfSize(future_text[0]) : sequence_value.size();
        }

        return 0;
    }

    CCL_INLINE auto Sequence::isStringEnd(TextIterator &rule_iterator, bool is_escaping) const
        -> bool
    {
        if (is_escaping) {
            return false;
        }

        auto text = rule_iterator.getRemainingWithCurrent();
        return text.startsWith(str_end);
    }

    CCL_INLINE auto Sequence::checkForUnexpectedEnd(
        TextIterator &rule_iterator, bool is_escaping, char32_t chr) const -> void
    {
        if (is_escaping) {
            return;
        }

        if (isEoF(chr)) {
            throwUnterminatedString(rule_iterator, "unterminated sequence");
        }

        if (land(chr == '\n', not sequence_flags.multiline)) {
            auto message = "new line is reached, but sequence has not been terminated"_sv;
            auto suggestion = fmt::format("use multiline sequence or close it with `{}`", str_end);

            throwUnterminatedString(rule_iterator, message, suggestion);
        }
    }

    CCL_INLINE auto Sequence::skipStringDefinition(TextIterator &rule_iterator) const -> void
    {
        rule_iterator.skip(str_begin.size() - 1);
    }

    CCL_INLINE auto Sequence::checkSequenceArguments(TextIterator &rule_iterator) const -> void
    {
        auto text = rule_iterator.getRemainingWithCurrent();

        if (str_begin.empty()) {
            throwEmptyStringBegin(rule_iterator);
        }

        if (str_end.empty()) {
            throwEmptyStringEnd(rule_iterator);
        }

        if (not text.startsWith(str_begin)) {
            throwStringBeginException(rule_iterator);
        }
    }

    auto Sequence::throwEmptyStringBegin(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError("sequence item begin cannot be empty"_sv);
        throw UnrecoverableError{ "unrecoverable error in SequenceType" };
    }

    auto Sequence::throwEmptyStringEnd(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError("sequence item end cannot be empty"_sv);
        throw UnrecoverableError{ "unrecoverable error in SequenceType" };
    }

    auto Sequence::throwUnterminatedString(
        TextIterator &rule_iterator,
        const string_view &message,
        const string_view &suggestion) -> void
    {
        rule_iterator.throwPanicError(message, suggestion);
        throw UnrecoverableError{ "unrecoverable error in SequenceType" };
    }

    auto Sequence::throwStringBeginException(TextIterator &rule_iterator) const -> void
    {
        auto message = fmt::format("string literal must begin with {}", str_begin);

        rule_iterator.throwPanicError(message);
        throw UnrecoverableError{ "unrecoverable error in SequenceType" };
    }
}// namespace ccl::lex::dot_item
