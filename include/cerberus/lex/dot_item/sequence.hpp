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
        using typename BasicItem::CommentTokens;
        using typename BasicItem::ExceptionAccumulator;
        using typename BasicItem::TextIterator;

    public:
        struct SequenceFlags
        {
            bool multiline : 1 = false;
            bool no_escaping_symbols : 1 = false;
        };

        Sequence(
            SequenceFlags flags_, u8string_view str_begin_, u8string_view str_end,
            TextIterator &rule_iterator_, AnalysisShared &analysis_shared_);

        Sequence(
            SequenceFlags flags_, u8string_view str_begin_, TextIterator &rule_iterator_,
            AnalysisShared &analysis_shared_)
          : Sequence(flags_, str_begin_, str_begin_, rule_iterator_, analysis_shared_)
        {}

        [[nodiscard]] auto get() const noexcept -> const std::u8string &
        {
            return sequence_value;
        }

        [[nodiscard]] auto getByRef() noexcept -> std::u8string &
        {
            return sequence_value;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override;

    private:
        [[nodiscard]] auto scanIteration(TextIterator &text_iterator, Token &token) const
            -> bool override;

        [[nodiscard]] auto isStringEnd(TextIterator &rule_iterator, bool is_escaping) const -> bool;

        auto skipStringDefinition(TextIterator &rule_iterator) const -> void;

        auto
            checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, char32_t chr) const
            -> void;
        auto checkSequenceArguments(TextIterator &rule_iterator) const -> void;

        static auto throwEmptyStringEnd(TextIterator &rule_iterator) -> void;
        static auto throwEmptyStringBegin(TextIterator &rule_iterator) -> void;
        auto throwStringBeginException(TextIterator &rule_iterator) const -> void;
        static auto throwUnterminatedString(
            TextIterator &rule_iterator,
            u8string_view message,
            u8string_view suggestion = {}) -> void;

        u8string_view str_begin{};
        u8string_view str_end{};
        std::u8string sequence_value{};
        SequenceFlags sequence_flags{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_STRING_HPP */
