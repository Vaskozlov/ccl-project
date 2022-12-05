#include <ccl/lex/dot_item/sequence.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    Sequence::Sequence(
        SequenceFlags sequence_flags, const string_view &sequence_begin,
        const string_view &sequence_end, TextIterator &rule_iterator, Id item_id)
      : BasicItem{item_id}
      , sequenceBegin{sequence_begin}
      , sequenceEnd{sequence_end}
    {
        flags.sequenceIsMultiline = sequence_flags.multiline;
        flags.sequenceNoEscapingSymbols = sequence_flags.noEscapingSymbols;

        auto begin_iterator_state = rule_iterator;

        checkSequenceArguments(rule_iterator);
        skipStringDefinition(rule_iterator);

        while (true) {
            auto chr = U'\0';
            auto is_escaping = false;

            if (flags.sequenceNoEscapingSymbols) {
                chr = rule_iterator.next();
            } else {
                auto [escaping, character] =
                    rule_iterator.nextRawCharWithEscapingSymbols({{U'[', U'['}, {U']', U']'}});
                is_escaping = escaping;
                chr = character;
            }

            checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

            if (isStringEnd(rule_iterator, is_escaping)) {
                rule_iterator.skip(sequenceEnd.size() - 1);
                break;
            }

            utf8::appendUtf32ToUtf8Container(sequenceValue, chr);
        }
    }

    auto Sequence::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        auto future_text = text_iterator.getFutureRemaining();

        if (future_text.startsWith(sequenceValue) ^ isReversed()) {
            return isReversed() ? utf8::size(future_text[0]) : sequenceValue.size();
        }

        return {};
    }

    CCL_INLINE auto Sequence::isStringEnd(TextIterator &rule_iterator, bool is_escaping) const
        -> bool
    {
        if (is_escaping) {
            return false;
        }

        auto text = rule_iterator.getRemainingWithCurrent();
        return text.startsWith(sequenceEnd);
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

        if (land('\n' == chr, !flags.sequenceIsMultiline)) [[unlikely]] {
            constexpr auto message = "new line is reached, but sequence has not been terminated"_sv;
            auto suggestion =
                fmt::format("use multiline sequence or close it with `{}`", sequenceEnd);

            throwUnterminatedString(rule_iterator, message, suggestion);
        }
    }

    CCL_INLINE auto Sequence::skipStringDefinition(TextIterator &rule_iterator) const -> void
    {
        rule_iterator.skip(sequenceBegin.size() - 1);
    }

    CCL_INLINE auto Sequence::checkSequenceArguments(TextIterator &rule_iterator) const -> void
    {
        auto text = rule_iterator.getRemainingWithCurrent();

        if (sequenceBegin.empty()) [[unlikely]] {
            throwEmptyStringBegin(rule_iterator);
        }

        if (sequenceEnd.empty()) [[unlikely]] {
            throwEmptyStringEnd(rule_iterator);
        }

        if (!text.startsWith(sequenceBegin)) [[unlikely]] {
            throwStringBeginException(rule_iterator);
        }
    }

    auto Sequence::throwEmptyStringBegin(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError(
            AnalysisStage::LEXICAL_ANALYSIS, "sequence item begin cannot be empty"_sv);
        throw UnrecoverableError{"unrecoverable error in SequenceType"};
    }

    auto Sequence::throwEmptyStringEnd(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError(
            AnalysisStage::LEXICAL_ANALYSIS, "sequence item end cannot be empty"_sv);
        throw UnrecoverableError{"unrecoverable error in SequenceType"};
    }

    auto Sequence::throwUnterminatedString(
        TextIterator &rule_iterator,
        const string_view &message,
        const string_view &suggestion) -> void
    {
        rule_iterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in SequenceType"};
    }

    auto Sequence::throwStringBeginException(TextIterator &rule_iterator) const -> void
    {
        auto message = fmt::format("string literal must begin with {}", sequenceBegin);

        rule_iterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message);
        throw UnrecoverableError{"unrecoverable error in SequenceType"};
    }
}// namespace ccl::lex::dot_item
