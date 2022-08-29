#include <ccl/lex/dot_item/sequence.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    Sequence::Sequence(
        SequenceFlags flags_, string_view str_begin_, string_view str_end_,
        TextIterator &rule_iterator_, AnalysisShared &analysis_shared_)
      : BasicItem(analysis_shared_), str_begin(str_begin_), str_end(str_end_),
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

    auto Sequence::empty() const noexcept -> bool
    {
        return sequence_value.empty();
    }

    auto Sequence::scanIteration(TextIterator &text_iterator, Token & /* unused */) const -> bool
    {
        auto future_text = text_iterator.getFutureRemaining(1);

        if (future_text.startsWith(sequence_value)) {
            text_iterator.skip(sequence_value.size());
            return true;
        }

        return false;
    }

    auto Sequence::isStringEnd(TextIterator &rule_iterator, bool is_escaping) const -> bool
    {
        if (is_escaping) {
            return false;
        }

        auto text = rule_iterator.getRemainingWithCurrent();
        return text.startsWith(str_end);
    }

    auto Sequence::checkForUnexpectedEnd(
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
            auto suggestion =
                ::fmt::format("use multiline sequence or close it with `{}`", str_end);

            throwUnterminatedString(rule_iterator, message, suggestion);
        }
    }

    auto Sequence::skipStringDefinition(TextIterator &rule_iterator) const -> void
    {
        rule_iterator.skip(str_begin.size() - 1);
    }

    auto Sequence::checkSequenceArguments(TextIterator &rule_iterator) const -> void
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
        rule_iterator.throwError("sequence item begin cannot be empty"_sv);
        throw UnrecoverableError{ "unreachable error in SequenceType" };
    }

    auto Sequence::throwEmptyStringEnd(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwError("sequence item end cannot be empty"_sv);
        throw UnrecoverableError{ "unreachable error in SequenceType" };
    }

    auto Sequence::throwUnterminatedString(
        TextIterator &rule_iterator,
        string_view message,
        string_view suggestion) -> void
    {
        rule_iterator.throwError(message, suggestion);
        throw UnrecoverableError{ "unreachable error in SequenceType" };
    }

    auto Sequence::throwStringBeginException(TextIterator &rule_iterator) const -> void
    {
        auto message = ::fmt::format("string literal must begin with {}", str_begin);

        rule_iterator.throwError(message);
        throw UnrecoverableError{ "unreachable error in SequenceType" };
    }
}// namespace ccl::lex::dot_item
