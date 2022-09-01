#ifndef CCL_PROJECT_BASIC_ITEM_HPP
#define CCL_PROJECT_BASIC_ITEM_HPP

#include <ccl/lex/analysis_shared.hpp>
#include <ccl/lex/dot_item/recurrence.hpp>
#include <ccl/lex/token.hpp>
#include <ccl/text/text_iterator.hpp>
#include <optional>

namespace ccl::lex::dot_item
{
    class BasicItem
    {
    public:
        using TextIterator = text::TextIterator;
        using CommentTokens = text::CommentTokens;

        explicit BasicItem(AnalysisShared &analysis_shared_) noexcept
          : analysis_shared{ analysis_shared_ }
        {}

        BasicItem(const BasicItem &) = default;
        BasicItem(BasicItem &&) noexcept = default;

        virtual ~BasicItem() = default;

        auto operator=(const BasicItem &) -> void = delete;
        auto operator=(BasicItem &&) noexcept -> void = delete;

        [[nodiscard]] auto getRecurrence() const noexcept -> Recurrence
        {
            return recurrence;
        }

        [[nodiscard]] auto isReversed() const noexcept -> bool
        {
            return reversed;
        }

        [[nodiscard]] auto hasPrefix() const noexcept -> bool
        {
            return prefix;
        }

        [[nodiscard]] auto hasPostfix() const noexcept -> bool
        {
            return postfix;
        }

        void setPrefix() noexcept
        {
            prefix = true;
        }

        void setPostfix() noexcept
        {
            postfix = true;
        }

        auto reverse() noexcept -> void
        {
            reversed = !reversed;
        }

        auto setRecurrence(Recurrence new_recurrence) noexcept -> void
        {
            recurrence = new_recurrence;
        }

        [[nodiscard]] auto canBeOptimized() const noexcept -> bool
        {
            return not reversed && recurrence.from == 0 && empty();
        }

        static auto alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void;
        static auto neverRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void;

        [[nodiscard]] virtual auto empty() const noexcept -> bool = 0;

        [[nodiscard]] auto scan(
            const TextIterator &text_iterator, const Token &token, bool main_scan = false) const
            -> std::optional<std::pair<TextIterator, Token>>;

    private:
        [[nodiscard]] auto scanIterationCall(TextIterator &local_iterator, Token &local_token) const
            -> bool;

        [[nodiscard]] virtual auto scanIteration(TextIterator &text_iterator, Token &token) const
            -> bool = 0;

        [[nodiscard]] auto successfullyScanned(
            const TextIterator &text_iterator, size_t times, bool main_scan) const -> bool;

        auto modifyToken(
            const TextIterator &before_scan_iterator, const TextIterator &after_scan_iterator,
            Token &token) const -> void;

    protected:
        Recurrence recurrence{ Recurrence::basic() };
        AnalysisShared &analysis_shared;
        bool reversed{ false };
        bool prefix{};
        bool postfix{};
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_BASIC_ITEM_HPP */
