#ifndef CCL_PROJECT_BASIC_ITEM_HPP
#define CCL_PROJECT_BASIC_ITEM_HPP

#include <ccl/lex/dot_item/repetition.hpp>
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
        using ScanResult = std::optional<Pair<TextIterator, Token>>;

        class SpecialItems;

        struct CCL_TRIVIAL_ABI Flags
        {
            bool reversed : 1 = false;
            bool isPrefix : 1 = false;
            bool isPostfix : 1 = false;
            bool sequenceIsMultiline : 1 = false;
            bool sequenceNoEscapingSymbols : 1 = false;
        };

    protected:
        Repetition repetition{ Repetition::basic() };
        size_t id{};
        Flags flags;

    public:
        explicit BasicItem(const size_t id_) : id(id_)
        {}

        BasicItem(const BasicItem &) = default;
        BasicItem(BasicItem &&) noexcept = default;

        virtual ~BasicItem() = default;

        auto operator=(const BasicItem &) -> void = delete;
        auto operator=(BasicItem &&) noexcept -> void = delete;

        [[nodiscard]] auto getRepetition() const noexcept -> Repetition
        {
            return repetition;
        }

        [[nodiscard]] auto isReversed() const noexcept -> bool
        {
            return flags.reversed;
        }

        [[nodiscard]] auto hasPrefix() const noexcept -> bool
        {
            return flags.isPrefix;
        }

        [[nodiscard]] auto hasPostfix() const noexcept -> bool
        {
            return flags.isPostfix;
        }

        void setPrefix() noexcept
        {
            flags.isPrefix = true;
        }

        void setPostfix() noexcept
        {
            flags.isPostfix = true;
        }

        auto reverse() noexcept -> void
        {
            flags.reversed = !flags.reversed;
        }

        auto setRepetition(Repetition new_repetition) noexcept -> void
        {
            repetition = new_repetition;
        }

        [[nodiscard]] auto canBeOptimized() const noexcept -> bool
        {
            return !isReversed() && repetition.from == 0 && empty();
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
    };
}// namespace ccl::lex::dot_item

namespace ccl::lex
{
    using SpecialItems = dot_item::BasicItem::SpecialItems;
}

#endif /* CCL_PROJECT_BASIC_ITEM_HPP */
