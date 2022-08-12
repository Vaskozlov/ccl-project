#ifndef CERBERUS_PROJECT_DOT_ITEM_STRING_HPP
#define CERBERUS_PROJECT_DOT_ITEM_STRING_HPP

#include <cerberus/lex/dot_item/basic_item.hpp>
#include <fmt/format.h>

namespace cerb::lex::dot_item
{
    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(SequenceException, text::TextIteratorException<CharT>);

    template<CharacterLiteral CharT>
    class Sequence : public BasicItem<CharT>
    {
    private:
        using Base = BasicItem<CharT>;
        using Base::isNextCharNotForScanning;

        using typename Base::CommentTokens;
        using typename Base::ExceptionAccumulator;
        using typename Base::ScanStatus;
        using typename Base::TextIterator;

    public:
        CERBLIB_DECL auto get() const -> const Str<CharT> &
        {
            return string;
        }

        CERBLIB_DECL auto getRef() -> Str<CharT> &
        {
            return string;
        }

        CERBLIB_DECL auto empty() const noexcept -> bool override
        {
            return string.empty();
        }

        constexpr Sequence(
            bool multiline_, StrView<CharT> str_token_, TextIterator &rule_iterator_,
            AnalysisShared<CharT> &analysis_shared_)
          : Base(analysis_shared_), str_token(str_token_), multiline(multiline_)
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

                string.push_back(chr);
            }

            addWarningIfEmpty(rule_iterator);
        }

        CERBLIB_DERIVED_CONSTRUCTORS(Sequence);

    private:
        CERBLIB_DECL auto scanIteration(TextIterator &text_iterator) const -> bool override
        {
            auto future_text = text_iterator.getRemainingFuture(1);

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

        constexpr auto
            checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, CharT chr) const
            -> void
        {
            using namespace cerb::string_view_literals;

            if (is_escaping) {
                return;
            }

            if (isEoF(chr)) {
                throwUnterminatedString(rule_iterator, "unterminated sequence");
            }

            if (land(chr == '\n', not multiline)) {
                auto message = "new line is reached, but sequence has not been terminated"_sv;
                auto suggestion =
                    fmt::format<"use multiline sequence or close it with `{}`">(str_token);

                throwUnterminatedString(rule_iterator, message, suggestion);
            }
        }

        constexpr auto skipStringDefinition(TextIterator &rule_iterator) const -> void
        {
            rule_iterator.rawSkip(str_token.size() - 1);
        }

        constexpr auto checkStringBegin(TextIterator &rule_iterator) const -> void
        {
            auto text = rule_iterator.getRemaining();

            if (str_token.empty()) {
                throwEmptyStringBegin(rule_iterator);
            }

            if (text.substr(0, str_token.size()) != str_token) {
                throwStringBeginException(rule_iterator);
            }
        }

        constexpr auto addWarningIfEmpty(TextIterator &rule_iterator) -> void
        {
            using namespace string_view_literals;

            if (string.empty()) {
                rule_iterator.throwWarning(SequenceException<CharT>{
                    rule_iterator, "empty string should not be used"_sv });
            }
        }

        constexpr static auto throwEmptyStringBegin(TextIterator &rule_iterator) -> void
        {
            using namespace std::string_view_literals;

            rule_iterator.throwException(
                SequenceException<CharT>{ rule_iterator, "sequence item begin cannot be empty" });
            throw UnrecoverableError{ "unreachable error in SequenceType" };
        }

        constexpr static auto throwUnterminatedString(
            TextIterator &rule_iterator,
            string_view message,
            string_view suggestion = {}) -> void
        {
            rule_iterator.throwException(
                SequenceException<CharT>{ rule_iterator, message, suggestion });
            throw UnrecoverableError{ "unreachable error in SequenceType" };
        }

        constexpr auto throwStringBeginException(TextIterator &rule_iterator) const -> void
        {
            auto message = fmt::format<CharT, "string literal must begin with {}">(str_token);

            rule_iterator.throwException(SequenceException<CharT>{ rule_iterator, message });
            throw UnrecoverableError{ "unreachable error in SequenceType" };
        }

        StrView<CharT> str_token{};
        Str<CharT> string{};
        bool multiline{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_STRING_HPP */
