#ifndef CCL_PROJECT_CONTAINER_STRING_HPP
#define CCL_PROJECT_CONTAINER_STRING_HPP

#include <ccl/lex/dot_item/basic_item.hpp>

namespace ccl::lex::dot_item
{
    class Sequence final : public BasicItem
    {
    public:
        struct CCL_TRIVIAL_ABI SequenceFlags
        {
            bool multiline : 1 = false;
            bool no_escaping_symbols : 1 = false;
        };

    private:
        using typename BasicItem::TextIterator;

        std::string sequence_value{};
        string_view str_begin{};
        string_view str_end{};
        SequenceFlags sequence_flags{};

    public:
        Sequence(
            SequenceFlags flags_, const string_view &str_begin_, const string_view &str_end,
            TextIterator &rule_iterator_, size_t id_ = 0);

        Sequence(
            SequenceFlags flags_, const string_view &str_begin_, TextIterator &rule_iterator_,
            size_t id_ = 0)
          : Sequence(flags_, str_begin_, str_begin_, rule_iterator_, id_)
        {}

        [[nodiscard]] auto getValue() noexcept -> std::string &
        {
            return sequence_value;
        }

        [[nodiscard]] auto getValue() const noexcept -> const std::string &
        {
            return sequence_value;
        }

        [[nodiscard]] auto empty() const noexcept -> bool final
        {
            return sequence_value.empty();
        }

    private:
        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> size_t final;

        [[nodiscard]] auto isStringEnd(TextIterator &rule_iterator, bool is_escaping) const -> bool;

        auto skipStringDefinition(TextIterator &rule_iterator) const -> void;

        auto
            checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, char32_t chr) const
            -> void;

        auto checkSequenceArguments(TextIterator &rule_iterator) const -> void;

        auto throwStringBeginException(TextIterator &rule_iterator) const -> void;

        CCL_INLINE static auto throwEmptyStringEnd(TextIterator &rule_iterator) -> void;

        CCL_INLINE static auto throwEmptyStringBegin(TextIterator &rule_iterator) -> void;

        CCL_INLINE static auto throwUnterminatedString(
            TextIterator &rule_iterator,
            const string_view &message,
            const string_view &suggestion = {}) -> void;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_CONTAINER_STRING_HPP */
