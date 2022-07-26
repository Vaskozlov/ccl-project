#ifndef CERBERUS_PROJECT_DOT_ITEM_STRING_HPP
#define CERBERUS_PROJECT_DOT_ITEM_STRING_HPP

#include <cerberus/lex/dot_item/basic_item.hpp>
#include <fmt/format.h>

namespace cerb::lex::dot_item
{
    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(StringException, text::TextIteratorException<CharT>);

    template<CharacterLiteral CharT>
    class String : public BasicItem<CharT>
    {
    private:
        using Base = BasicItem<CharT>;
        using typename Base::CommentTokens;
        using typename Base::ExceptionAccumulator;
        using typename Base::TextIterator;

    public:
        CERBLIB_DECL auto get() const -> const Str<CharT> &
        {
            return string;
        }

        constexpr String(bool multiline_, StrView<CharT> str_token_, TextIterator &rule_iterator_)
          : str_token(str_token_), multiline(multiline_)
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

        CERBLIB_DERIVED_CONSTRUCTORS(String);

    private:
        constexpr auto isStringEnd(TextIterator &rule_iterator, bool is_escaping) const -> bool
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
            using namespace std::string_literals;

            if (is_escaping) {
                return;
            }

            if (isEoF(chr)) {
                throwUnterminatedString(rule_iterator, "unterminated string item");
            }

            if (land(chr == '\n', not multiline)) {
                auto message = "New line reached, but string literal has not been terminated"s;
                auto suggestion =
                    fmt::format<"use multiline option or close string literal with {}">(str_token);

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
                throwEmptyStringBegin();
            }

            if (text.substr(0, str_token.size()) != str_token) {
                throwStringBeginException(rule_iterator);
            }
        }

        constexpr auto addWarningIfEmpty(TextIterator &rule_iterator) -> void
        {
            if (string.empty()) {
                auto error =
                    StringException<CharT>(rule_iterator, "empty string should not be used");
                rule_iterator.throwWarning(std::move(error));
            }
        }

        constexpr static auto throwEmptyStringBegin() -> void
        {
            throw LogicError("string literal begin can not be empty");
        }

        constexpr static auto throwUnterminatedString(
            TextIterator &rule_iterator,
            string_view message,
            string_view suggestion = {}) -> void
        {
            auto error = StringException<CharT>(rule_iterator, message, suggestion);
            rule_iterator.throwException(std::move(error));
        }

        constexpr auto throwStringBeginException(TextIterator &rule_iterator) const -> void
        {
            auto error = StringException<CharT>(
                rule_iterator, fmt::format<CharT, "string literal must begin with {}">(str_token));
            rule_iterator.throwException(std::move(error));
        }

        StrView<CharT> str_token{};
        Str<CharT> string{};
        bool multiline{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_STRING_HPP */
