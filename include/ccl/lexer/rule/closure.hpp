#ifndef CCL_PROJECT_CLOSURE_HPP
#define CCL_PROJECT_CLOSURE_HPP

#include <ccl/text/text_iterator.hpp>

namespace ccl::lexer::rule
{
    class CCL_TRIVIAL_ABI Closure
    {
    public:
        std::size_t from{};
        std::size_t to{};

        CCL_DECL
            Closure(const std::size_t repetition_begin, const std::size_t repetition_end) noexcept
          : from{repetition_begin}
          , to{repetition_end}
        {}

        [[nodiscard]] explicit Closure(text::TextIterator &text_iterator);

        [[nodiscard]] constexpr static auto max() noexcept -> std::size_t
        {
            return std::numeric_limits<std::size_t>::max();
        }

        [[nodiscard]] auto isInClosure(const std::size_t value) const noexcept -> bool
        {
            return value >= from && value <= to;
        }

        [[nodiscard]] auto
            operator<=>(const Closure &) const noexcept -> std::strong_ordering = default;

    private:
        static auto checkRangeStart(const text::TextIterator &text_iterator) -> void;

        [[nodiscard]] static auto
            parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> std::size_t;

        auto checkCorrectnessOfValues(const text::TextIterator &text_iterator) const -> void;

        auto throwBadValues(const text::TextIterator &text_iterator) const -> void;

        [[noreturn]] static auto
            throwRangeBeginException(const text::TextIterator &text_iterator) -> void;

        [[noreturn]] static auto
            throwUnexpectedCharacter(const text::TextIterator &text_iterator, char32_t chr) -> void;
    };
}// namespace ccl::lexer::rule

#endif /* CCL_PROJECT_CLOSURE_HPP */
