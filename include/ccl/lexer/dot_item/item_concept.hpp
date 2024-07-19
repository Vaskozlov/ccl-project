#ifndef CCL_PROJECT_ITEM_CONCEPT_HPP
#define CCL_PROJECT_ITEM_CONCEPT_HPP

#include <ccl/lexer/dot_item/repetition.hpp>
#include <ccl/lexer/parsing_result.hpp>
#include <ccl/lexer/scan_result.hpp>
#include <ccl/lexer/token.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/text/text_iterator.hpp>
#include <isl/memory.hpp>

namespace ccl::lexer::dot_item
{
    class Container;

    class DotItemConcept
    {
    public:
        using TextIterator = text::TextIterator;
        using ForkedGenerator = typename text::TextIterator::ForkedTextIterator;

        class AnyPlaceItems;

        struct CCL_TRIVIAL_ABI Flags
        {
            bool reversed{};
            bool isPrefix{};
            bool isPostfix{};
            bool sequenceIsMultiline{};
            bool noEscapingSymbols{};
        };

    private:
        Closure closure{1, 1};
        Id id{};
        std::size_t bindingPower{};
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

        [[nodiscard]] auto getClosure() const noexcept -> Closure
        {
            return closure;
        }

        [[nodiscard]] auto getFlags() const noexcept -> Flags
        {
            return flags;
        }

        [[nodiscard]] auto getBindingPower() const noexcept -> std::size_t
        {
            return bindingPower;
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

        auto setRepetition(Closure new_repetition) noexcept -> void
        {
            closure = new_repetition;
        }

        auto setBindingPower(std::size_t new_binding_power) noexcept -> void
        {
            bindingPower = new_binding_power;
        }

        [[nodiscard]] auto canBeOptimized() const noexcept -> bool
        {
            const auto fromZeroAccuracies = closure.from == 0;
            return !isReversed() && fromZeroAccuracies && empty();
        }

        [[nodiscard]] auto getId() const noexcept -> Id
        {
            return id;
        }

        static auto alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void;
        static auto neverRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void;

        [[nodiscard]] auto scan(ForkedGenerator text_iterator) const -> ScanResult;
        [[nodiscard]] auto parse(ForkedGenerator text_iterator) const -> ParsingResult;

        [[nodiscard]] virtual auto empty() const noexcept -> bool = 0;

        [[nodiscard]] virtual auto
            scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult = 0;

        [[nodiscard]] virtual auto
            parseIteration(const ForkedGenerator &text_iterator) const -> ParsingResult = 0;
    };

    class DotItem : public isl::UniquePtr<DotItemConcept>
    {
    public:
        using isl::UniquePtr<DotItemConcept>::unique_ptr;

        template<std::derived_from<DotItemConcept> T>
        explicit constexpr DotItem(T dot_item)
          : isl::UniquePtr<DotItemConcept>::unique_ptr(isl::makeUnique<T>(std::move(dot_item)))
        {}
    };
}// namespace ccl::lexer::dot_item

namespace ccl::lexer
{
    using AnyPlaceItems = dot_item::DotItemConcept::AnyPlaceItems;
}

#endif /* CCL_PROJECT_ITEM_CONCEPT_HPP */
