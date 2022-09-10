#ifndef CCL_PROJECT_BASIC_ITEM_HPP
#define CCL_PROJECT_BASIC_ITEM_HPP

#include <ccl/lex/dot_item/recurrence.hpp>
#include <ccl/lex/token.hpp>
#include <ccl/text/text_iterator.hpp>
#include <optional>

namespace ccl::lex::dot_item
{
    // NOLINTNEXTLINE
    CCL_ENUM(ScanType, u16, MAIN, FORKED, SPECIAL);

    class Container;

    class BasicItem
    {
    public:
        using TextIterator = text::TextIterator;
        using ForkedGenerator = typename text::TextIterator::ForkedTextIterator;
        using BasicItemPtr = std::unique_ptr<BasicItem>;
        using ScanResult = std::optional<std::pair<TextIterator, Token>>;

        struct SpecialItems;

        BasicItem(SpecialItems &special_items_, size_t id_) : id(id_), special_items(special_items_)
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
            reversed = not reversed;
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
        [[nodiscard]] auto scan(ForkedGenerator text_iterator) const -> std::optional<size_t>;

    private:
        [[nodiscard]] virtual auto scanIteration(const ForkedGenerator &text_iterator) const
            -> size_t = 0;

    protected:
        Recurrence recurrence{ Recurrence::basic() };
        size_t id{};
        SpecialItems &special_items;
        bool reversed{ false };
        bool prefix{};
        bool postfix{};
    };
}// namespace ccl::lex::dot_item

namespace ccl::lex
{
    using SpecialItems = dot_item::BasicItem::SpecialItems;
}

#endif /* CCL_PROJECT_BASIC_ITEM_HPP */
