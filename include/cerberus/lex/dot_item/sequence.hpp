#ifndef CERBERUS_PROJECT_DOT_ITEM_STRING_HPP
#define CERBERUS_PROJECT_DOT_ITEM_STRING_HPP

#include <cerberus/lex/dot_item/basic_item.hpp>
#include <fmt/format.h>

namespace cerb::lex::dot_item
{
    CERBLIB_EXCEPTION(SequenceException, text::TextIteratorException);

    class Sequence : public BasicItem
    {
    private:
        using BasicItem::isNextCharNotForScanning;

        using typename BasicItem::CommentTokens;
        using typename BasicItem::ExceptionAccumulator;
        using typename BasicItem::ScanStatus;
        using typename BasicItem::TextIterator;

    public:
        [[nodiscard]] auto get() const -> const std::u8string &
        {
            return string;
        }

        [[nodiscard]] auto getRef() -> std::u8string &
        {
            return string;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return string.empty();
        }

        Sequence(
            bool multiline_, u8string_view str_token_, TextIterator &rule_iterator_,
            AnalysisShared &analysis_shared_)
          : BasicItem(analysis_shared_), str_token(str_token_), multiline(multiline_)
        {
            auto &rule_iterator = rule_iterator_;
            auto begin_iterator_state = rule_iterator;

            checkStringBegin(rule_iterator);
            skipStringDefinition(rule_iterator);

            while (true) {
                auto [is_escaping, chr] = rule_iterator.nextRawCharWithEscapingSymbols();

                checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

                if (isStringEnd(rule_iterator, is_escaping)) {
                    break;
                }

                utf8::addUtf32ToUtf8String(string, chr);
            }

            addWarningIfEmpty(rule_iterator);
        }

    private:
        [[nodiscard]] auto scanIteration(TextIterator &text_iterator) const -> bool override
        {
            auto future_text = text_iterator.getRemainingFutureAfterSymbols(1);

            if (future_text.substr(0, string.size()) == string) {
                text_iterator.rawSkip(string.size());
                return true;
            }

            return false;
        }

        CERBLIB_DECL auto isStringEnd(TextIterator &rule_iterator, bool is_escaping) const -> bool
        {
            if (is_escaping) {
                return false;
            }

            auto text = rule_iterator.getRemaining();
            return text.substr(0, str_token.size()) == str_token;
        }

        auto
            checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, char32_t chr) const
            -> void
        {
            using namespace std::string_view_literals;

            if (is_escaping) {
                return;
            }

            if (isEoF(chr)) {
                throwUnterminatedString(rule_iterator, u8"unterminated sequence");
            }

            if (land(chr == '\n', not multiline)) {
                auto message = u8"new line is reached, but sequence has not been terminated"sv;
                auto suggestion =
                    fmt::format<u8"use multiline sequence or close it with `{}`">(str_token);

                throwUnterminatedString(rule_iterator, message, suggestion);
            }
        }

        auto skipStringDefinition(TextIterator &rule_iterator) const -> void
        {
            rule_iterator.rawSkip(str_token.size() - 1);
        }

        auto checkStringBegin(TextIterator &rule_iterator) const -> void
        {
            auto text = rule_iterator.getRemaining();

            if (str_token.empty()) {
                throwEmptyStringBegin(rule_iterator);
            }

            if (text.substr(0, str_token.size()) != str_token) {
                throwStringBeginException(rule_iterator);
            }
        }

        auto addWarningIfEmpty(TextIterator &rule_iterator) -> void
        {
            using namespace std::string_view_literals;

            if (string.empty()) {
                rule_iterator.template throwWarning<SequenceException>(
                    u8"empty string should not be used"sv);
            }
        }

        static auto throwEmptyStringBegin(TextIterator &rule_iterator) -> void
        {
            using namespace std::string_view_literals;

            rule_iterator.throwException<SequenceException>(
                u8"sequence item begin cannot be empty"sv);
            throw UnrecoverableError{ "unreachable error in SequenceType" };
        }

        static auto throwUnterminatedString(
            TextIterator &rule_iterator,
            std::u8string_view message,
            std::u8string_view suggestion = {}) -> void
        {
            rule_iterator.template throwException<SequenceException>(message, suggestion);
            throw UnrecoverableError{ "unreachable error in SequenceType" };
        }

        auto throwStringBeginException(TextIterator &rule_iterator) const -> void
        {
            auto message = fmt::format<u8"string literal must begin with {}">(str_token);

            rule_iterator.template throwException<SequenceException>(message);
            throw UnrecoverableError{ "unreachable error in SequenceType" };
        }

        u8string_view str_token{};
        std::u8string string{};
        bool multiline{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_STRING_HPP */
