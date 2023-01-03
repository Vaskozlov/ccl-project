#ifndef CCL_PROJECT_BASIC_ITEM_HPP
#define CCL_PROJECT_BASIC_ITEM_HPP

#include <ccl/lex/dot_item/repetition.hpp>
#include <ccl/lex/token.hpp>
#include <ccl/text/text_iterator.hpp>

namespace ccl::lex::dot_item
{
    enum struct ScanType : u16
    {
        MAIN,
        FORKED,
        SPECIAL
    };

    class Container;

    class BasicItem
    {
    public:
        using TextIterator = text::TextIterator;
        using ForkedGenerator = typename text::TextIterator::ForkedTextIterator;

        class SpecialItems;

        struct CCL_TRIVIAL_ABI Flags
        {
            bool reversed = false;
            bool isPrefix = false;
            bool isPostfix = false;
            bool sequenceIsMultiline = false;
            bool sequenceNoEscapingSymbols = false;
        };

    private:
        Repetition repetition{Repetition::basic()};
        Id id{};
        Flags flags;

    public:
        explicit BasicItem(Id item_id)
          : id{item_id}
        {}

        BasicItem(Id item_id, Flags item_flags)
          : id{item_id}
          , flags{item_flags}
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

        [[nodiscard]] auto getFlags() const noexcept -> Flags
        {
            return flags;
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
            return (!isReversed() && (0 == repetition.from)) && empty();
        }

        [[nodiscard]] auto getId() const noexcept -> Id
        {
            return id;
        }

        static auto alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void;
        static auto neverRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void;

        [[nodiscard]] virtual auto empty() const noexcept -> bool = 0;
        [[nodiscard]] auto scan(ForkedGenerator text_iterator) const -> Optional<size_t>;

    private:
        [[nodiscard]] virtual auto scanIteration(const ForkedGenerator &text_iterator) const
            -> size_t = 0;
    };

    class DotItem
    {
    private:
        UniquePtr<BasicItem> item{};

    public:
        DotItem() = default;

        template<std::derived_from<BasicItem> T>
        explicit DotItem(T item)
          : item{makeUnique<T>(std::move(item))}
        {}

        auto operator->() -> BasicItem *
        {
            return get();
        }

        auto operator->() const -> const BasicItem *
        {
            return get();
        }

        [[nodiscard]] auto get() -> BasicItem *
        {
            return item.get();
        }

        [[nodiscard]] auto get() const -> const BasicItem *
        {
            return item.get();
        }
    };
}// namespace ccl::lex::dot_item

namespace ccl::lex
{
    using SpecialItems = dot_item::BasicItem::SpecialItems;
}

#endif /* CCL_PROJECT_BASIC_ITEM_HPP */
