module;
#include <ccl/defines.hpp>
export module ccl.lexer.dot_item.core:repetition;

export import ccl.text;

export namespace ccl::lexer::dot_item
{
    class Repetition
    {
    public:
        std::size_t from{};
        std::size_t to{};

        [[nodiscard]] constexpr Repetition(std::size_t repetition_begin, std::size_t repetition_end) noexcept
          : from{repetition_begin}
          , to{repetition_end}
        {}

        [[nodiscard]] explicit Repetition(text::TextIterator &text_iterator);

        [[nodiscard]] consteval static auto basic() noexcept -> Repetition
        {
            return {1, 1};
        }

        [[nodiscard]] consteval static auto question() noexcept -> Repetition
        {
            return {0, 1};
        }

        [[nodiscard]] consteval static auto star() noexcept -> Repetition
        {
            return {0, max()};
        }

        [[nodiscard]] consteval static auto plus() noexcept -> Repetition
        {
            return {1, max()};
        }

        [[nodiscard]] consteval static auto max() noexcept -> std::size_t
        {
            return std::numeric_limits<std::size_t>::max();
        }

        [[nodiscard]] auto inRange(std::size_t value) const noexcept
        {
            return value >= from && value <= to;
        }

        [[nodiscard]] auto
            operator<=>(const Repetition &) const noexcept -> std::weak_ordering = default;

    private:
        [[nodiscard]] static auto
            parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> std::size_t;

        static auto checkRangeStart(text::TextIterator &text_iterator) -> void;

        auto checkCorrectnessOfValues(text::TextIterator &text_iterator) const -> void;

        auto throwBadValues(text::TextIterator &text_iterator) const -> void;

        [[noreturn]] static auto
            throwRangeBeginException(text::TextIterator &text_iterator) -> void;

        [[noreturn]] static auto
            throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr) -> void;
    };
}// namespace ccl::lexer::dot_item
