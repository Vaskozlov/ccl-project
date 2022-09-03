#ifndef CCL_PROJECT_BASIC_ITEM_HPP
#define CCL_PROJECT_BASIC_ITEM_HPP

#include <ccl/lex/dot_item/recurrence.hpp>
#include <ccl/lex/token.hpp>
#include <ccl/text/text_iterator.hpp>
#include <optional>

namespace ccl::lex::dot_item
{
    // NOLINTNEXTLINE
    CCL_ENUM(ScanType, u16, MAIN = 1, FORKED = 2, SPECIAL = 3);

    class BasicItem
    {
    public:
        using TextIterator = text::TextIterator;

        struct SpecialItems;

        explicit BasicItem(SpecialItems &special_items_, size_t id_ = 0) noexcept
          : id(id_), special_items{ special_items_ } {};

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

        [[nodiscard]] auto getId() const noexcept -> size_t
        {
            return id;
        }

        static auto alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void;
        static auto neverRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void;

        [[nodiscard]] virtual auto empty() const noexcept -> bool = 0;

        [[nodiscard]] auto scan(
            const TextIterator &text_iterator, const Token &token,
            ScanType main_scan = ScanType::FORKED) const
            -> std::optional<std::pair<TextIterator, Token>>;

        [[nodiscard]] auto
            scan(const TextIterator &text_iterator, ScanType main_scan = ScanType::FORKED) const
            -> std::optional<std::pair<TextIterator, Token>>
        {
            return scan(text_iterator, Token{ text_iterator, getId() }, main_scan);
        }

    private:
        [[nodiscard]] auto
            successfulIteration(TextIterator &local_iterator, Token &local_token) const -> bool;

        [[nodiscard]] virtual auto scanIteration(TextIterator &text_iterator, Token &token) const
            -> bool = 0;

        [[nodiscard]] auto isSuccessfullyScanned(
            const TextIterator &text_iterator, size_t times, ScanType scan_type) const -> bool;

        auto modifyToken(
            const TextIterator &before_scan_iterator, const TextIterator &after_scan_iterator,
            Token &token) const -> void;

    protected:
        Recurrence recurrence{ Recurrence::basic() };
        size_t id{};
        SpecialItems &special_items;
        bool reversed{ false };
        bool prefix{};
        bool postfix{};
    };

    struct BasicItem::SpecialItems
    {
        auto scan(const TextIterator &text_iterator) const
            -> std::optional<std::pair<TextIterator, Token>>
        {
            for (auto &&item : special_items) {
                auto scan_result = item->scan(text_iterator, ScanType::SPECIAL);

                if (scan_result.has_value() && not scan_result->second.getRepr().empty()) {
                    return scan_result;
                }
            }

            return std::nullopt;
        }

        std::vector<std::unique_ptr<BasicItem>> special_items;
    };
}// namespace ccl::lex::dot_item

namespace ccl::lex
{
    using SpecialItems = dot_item::BasicItem::SpecialItems;
}

#endif /* CCL_PROJECT_BASIC_ITEM_HPP */
