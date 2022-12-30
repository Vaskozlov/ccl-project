#ifndef CCL_PROJECT_SEQUENCE_HPP
#define CCL_PROJECT_SEQUENCE_HPP

#include <ccl/lex/dot_item/basic_item.hpp>

namespace ccl::lex::dot_item
{
    class Sequence final : public BasicItem
    {
    public:
        struct CCL_TRIVIAL_ABI SequenceFlags
        {
            bool multiline : 1 = false;
            bool noEscaping : 1 = false;
        };

    private:
        using typename BasicItem::TextIterator;

        std::string sequenceValue{};
        string_view starter{};
        string_view ender{};

    public:
        Sequence(
            SequenceFlags sequence_flags, const string_view &sequence_starter,
            const string_view &sequence_ender, TextIterator &rule_iterator, Id item_id = 0);

        Sequence(
            SequenceFlags sequence_flags, const string_view &sequence_begin_and_end,
            TextIterator &rule_iterator, Id item_id = 0)
          : Sequence{
                sequence_flags, sequence_begin_and_end, sequence_begin_and_end, rule_iterator,
                item_id}
        {}

        [[nodiscard]] auto getValue() noexcept -> std::string &
        {
            return sequenceValue;
        }

        [[nodiscard]] auto getValue() const noexcept -> const std::string &
        {
            return sequenceValue;
        }

        [[nodiscard]] auto empty() const noexcept -> bool final
        {
            return sequenceValue.empty();
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

        [[noreturn]] auto throwStringBeginException(TextIterator &rule_iterator) const -> void;

        [[noreturn]] CCL_INLINE static auto throwEmptyStringEnd(TextIterator &rule_iterator)
            -> void;

        [[noreturn]] CCL_INLINE static auto throwEmptyStringBegin(TextIterator &rule_iterator)
            -> void;

        [[noreturn]] CCL_INLINE static auto throwUnterminatedString(
            TextIterator &rule_iterator,
            const string_view &message,
            const string_view &suggestion = {}) -> void;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_SEQUENCE_HPP */
