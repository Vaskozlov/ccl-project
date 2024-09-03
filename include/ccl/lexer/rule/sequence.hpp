#ifndef CCL_PROJECT_SEQUENCE_HPP
#define CCL_PROJECT_SEQUENCE_HPP

#include <ccl/lexer/rule/rule_block_interface.hpp>

namespace ccl::lexer::rule
{
    class Sequence : public RuleBlockInterface
    {
    public:
        struct CCL_TRIVIAL_ABI SequenceFlags
        {
            bool multiline{};
            bool noEscaping{};
        };

        using RuleBlockInterface::TextIterator;

    private:
        std::string sequenceValue;
        isl::string_view starter;
        isl::string_view ender;

    public:
        [[nodiscard]] Sequence(
            SequenceFlags sequence_flags, isl::string_view sequence_starter,
            isl::string_view sequence_ender, TextIterator &rule_iterator);

        [[nodiscard]] Sequence(
            SequenceFlags sequence_flags, isl::string_view sequence_begin_and_end,
            TextIterator &rule_iterator);

        [[nodiscard]] explicit Sequence(std::string value);

        [[nodiscard]] auto getValue() noexcept CCL_LIFETIMEBOUND -> std::string &
        {
            return sequenceValue;
        }

        [[nodiscard]] auto getValue() const noexcept CCL_LIFETIMEBOUND -> const std::string &
        {
            return sequenceValue;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return sequenceValue.empty();
        }

    private:
        [[nodiscard]] auto
            scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult override;

        [[nodiscard]] auto
            isStringEnd(const TextIterator &rule_iterator, bool is_escaping) const -> bool;

        auto skipStringDefinition(TextIterator &rule_iterator) const -> void;

        auto checkForUnexpectedEnd(
            const TextIterator &rule_iterator, bool is_escaping, char32_t chr) const -> void;

        auto checkSequenceArguments(const TextIterator &rule_iterator) const -> void;

        [[noreturn]] auto
            throwStringBeginException(const TextIterator &rule_iterator) const -> void;

        [[noreturn]] static auto throwEmptyStringEnd(const TextIterator &rule_iterator) -> void;

        [[noreturn]] static auto throwEmptyStringBegin(const TextIterator &rule_iterator) -> void;

        [[noreturn]] static auto throwUnterminatedString(
            const TextIterator &rule_iterator,
            isl::string_view message,
            isl::string_view suggestion = {}) -> void;
    };
}// namespace ccl::lexer::rule

#endif /* CCL_PROJECT_SEQUENCE_HPP */
