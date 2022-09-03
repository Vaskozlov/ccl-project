#ifndef CCL_PROJECT_CONTAINER_STRING_HPP
#define CCL_PROJECT_CONTAINER_STRING_HPP

#include <ccl/lex/dot_item/basic_item.hpp>

namespace ccl::lex::dot_item
{
    class Sequence final : public BasicItem
    {
    private:
        using typename BasicItem::TextIterator;

    public:
        struct SequenceFlags
        {
            bool multiline : 1 = false;
            bool no_escaping_symbols : 1 = false;
        };

        Sequence(
            SequenceFlags flags_, string_view str_begin_, string_view str_end,
            TextIterator &rule_iterator_, SpecialItems &special_items_);

        Sequence(
            SequenceFlags flags_, string_view str_begin_, TextIterator &rule_iterator_,
            SpecialItems &special_items_)
          : Sequence(flags_, str_begin_, str_begin_, rule_iterator_, special_items_)
        {}

        [[nodiscard]] auto getValue() noexcept -> std::string &
        {
            return sequence_value;
        }

        [[nodiscard]] auto getValue() const noexcept -> const std::string &
        {
            return sequence_value;
        }

        [[nodiscard]] auto empty() const noexcept -> bool final;

    private:
        [[nodiscard]] auto scanIteration(TextIterator &text_iterator, Token &token) const
            -> bool final;

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
            string_view message,
            string_view suggestion = {}) -> void;

        string_view str_begin{};
        string_view str_end{};
        std::string sequence_value{};
        SequenceFlags sequence_flags{};
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_CONTAINER_STRING_HPP */
