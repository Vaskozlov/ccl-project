#ifndef CCL_PROJECT_SEQUENCE_HPP
#define CCL_PROJECT_SEQUENCE_HPP

#include <ccl/lexer/dot_item/item_concept.hpp>

namespace ccl::lexer::dot_item
{
    class Sequence final : public DotItemConcept
    {
    public:
        struct CCL_TRIVIAL_ABI SequenceFlags
        {
            bool multiline = false;
            bool noEscaping = false;
        };

    private:
        using typename DotItemConcept::TextIterator;

        std::string sequenceValue{};
        isl::string_view starter{};
        isl::string_view ender{};

    public:
        [[nodiscard]] Sequence(
            SequenceFlags sequence_flags, isl::string_view sequence_starter,
            isl::string_view sequence_ender, TextIterator &rule_iterator, Id item_id = 0);

        [[nodiscard]] Sequence(
            SequenceFlags sequence_flags, isl::string_view sequence_begin_and_end,
            TextIterator &rule_iterator, Id item_id = 0);

        [[nodiscard]] auto getValue() noexcept -> std::string &
        {
            return sequenceValue;
        }

        [[nodiscard]] auto getValue() const noexcept -> const std::string &
        {
            return sequenceValue;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return sequenceValue.empty();
        }

    private:
        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> std::optional<size_t> override;

        [[nodiscard]] auto isStringEnd(const TextIterator &rule_iterator, bool is_escaping) const
            -> bool;

        auto skipStringDefinition(TextIterator &rule_iterator) const -> void;

        auto
            checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, char32_t chr) const
            -> void;

        auto checkSequenceArguments(TextIterator &rule_iterator) const -> void;

        [[noreturn]] auto throwStringBeginException(TextIterator &rule_iterator) const -> void;

        [[noreturn]] static auto throwEmptyStringEnd(TextIterator &rule_iterator) -> void;

        [[noreturn]] static auto throwEmptyStringBegin(TextIterator &rule_iterator) -> void;

        [[noreturn]] static auto throwUnterminatedString(
            TextIterator &rule_iterator,
            isl::string_view message,
            isl::string_view suggestion = {}) -> void;
    };
}// namespace ccl::lexer::dot_item

#endif /* CCL_PROJECT_SEQUENCE_HPP */
