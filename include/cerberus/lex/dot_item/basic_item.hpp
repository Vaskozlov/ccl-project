#ifndef CERBERUS_PROJECT_BASIC_ITEM_HPP
#define CERBERUS_PROJECT_BASIC_ITEM_HPP

#include <cerberus/lex/analysis_shared.hpp>
#include <cerberus/lex/dot_item/recurrence.hpp>
#include <cerberus/lex/exception.hpp>
#include <cerberus/lex/token.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <optional>

namespace cerb::lex::dot_item
{
    class BasicItem
    {
    public:
        using TextIterator = text::TextIterator;
        using CommentTokens = text::CommentTokens;
        using ExceptionAccumulator = analysis::ExceptionAccumulator<text::TextIteratorException>;

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
            return recurrence.from == 0 && empty();
        }

        [[nodiscard]] virtual auto empty() const noexcept -> bool = 0;

        [[nodiscard]] auto isNextCharNotForScanning(const TextIterator &text_iterator) const
            -> bool;

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
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_BASIC_ITEM_HPP */
