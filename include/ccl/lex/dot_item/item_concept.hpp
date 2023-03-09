#ifndef CCL_PROJECT_ITEM_CONCEPT_HPP
#define CCL_PROJECT_ITEM_CONCEPT_HPP

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

    class DotItemConcept
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
        CCL_INLINE explicit DotItemConcept(Id item_id)
          : id{item_id}
        {}

        CCL_INLINE DotItemConcept(Id item_id, Flags item_flags)
          : id{item_id}
          , flags{item_flags}
        {}

        DotItemConcept(const DotItemConcept &) = default;
        DotItemConcept(DotItemConcept &&) noexcept = default;

        virtual ~DotItemConcept() = default;

        auto operator=(const DotItemConcept &) -> void = delete;
        auto operator=(DotItemConcept &&) noexcept -> void = delete;

        [[nodiscard]] auto getRepetition() const noexcept -> Repetition
        {
            return repetition;
        }

        [[nodiscard]] CCL_INLINE auto getFlags() const noexcept -> Flags
        {
            return flags;
        }

        [[nodiscard]] CCL_INLINE auto isReversed() const noexcept -> bool
        {
            return flags.reversed;
        }

        [[nodiscard]] CCL_INLINE auto hasPrefix() const noexcept -> bool
        {
            return flags.isPrefix;
        }

        [[nodiscard]] CCL_INLINE auto hasPostfix() const noexcept -> bool
        {
            return flags.isPostfix;
        }

        CCL_INLINE void setPrefix() noexcept
        {
            flags.isPrefix = true;
        }

        CCL_INLINE void setPostfix() noexcept
        {
            flags.isPostfix = true;
        }

        CCL_INLINE auto reverse() noexcept -> void
        {
            flags.reversed = !flags.reversed;
        }

        CCL_INLINE auto setRepetition(Repetition new_repetition) noexcept -> void
        {
            repetition = new_repetition;
        }

        [[nodiscard]] auto canBeOptimized() const noexcept -> bool
        {
            return (!isReversed() && (0 == repetition.from)) && empty();
        }

        [[nodiscard]] CCL_INLINE auto getId() const noexcept -> Id
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
        UniquePtr<DotItemConcept> item{};

    public:
        DotItem() = default;

        template<std::derived_from<DotItemConcept> T>
        explicit DotItem(T dot_item)
          : item{makeUnique<T>(std::move(dot_item))}
        {}

        CCL_INLINE auto operator->() -> DotItemConcept *
        {
            return get();
        }

        CCL_INLINE auto operator->() const -> const DotItemConcept *
        {
            return get();
        }

        [[nodiscard]] CCL_INLINE auto get() -> DotItemConcept *
        {
            return item.get();
        }

        [[nodiscard]] CCL_INLINE auto get() const -> const DotItemConcept *
        {
            return item.get();
        }
    };
}// namespace ccl::lex::dot_item

namespace ccl::lex
{
    using SpecialItems = dot_item::DotItemConcept::SpecialItems;
}

#endif /* CCL_PROJECT_ITEM_CONCEPT_HPP */
