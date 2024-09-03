#ifndef CCL_PROJECT_RULE_INTERFACE_HPP
#define CCL_PROJECT_RULE_INTERFACE_HPP

#include <ccl/lexer/rule/closure.hpp>
#include <ccl/lexer/scan_result.hpp>
#include <ccl/text/text_iterator.hpp>

namespace ccl::lexer::rule
{
    class Container;

    class RuleBlockInterface
    {
    public:
        using TextIterator = text::TextIterator;
        using ForkedGenerator = text::TextIterator::ForkedTextIterator;

        class AnyPlaceItems;

        struct CCL_TRIVIAL_ABI Flags
        {
            bool reversed{};
            bool extractable{};
            bool sequenceIsMultiline{};
            bool noEscapingSymbols{};
            bool isHiddenFromParser{};
        };

    private:
        Closure closure{1, 1};
        SmallId id{};
        Flags flags;

    public:
        RuleBlockInterface() = default;

        [[nodiscard]] explicit RuleBlockInterface(Flags item_flags)
          : flags{item_flags}
        {}

        RuleBlockInterface(const RuleBlockInterface &) = default;
        RuleBlockInterface(RuleBlockInterface &&) noexcept = default;

        virtual ~RuleBlockInterface() = default;

        auto operator=(const RuleBlockInterface &) -> void = delete;
        auto operator=(RuleBlockInterface &&) noexcept -> void = delete;

        [[nodiscard]] auto getClosure() const noexcept -> Closure
        {
            return closure;
        }

        [[nodiscard]] auto getFlags() const noexcept -> Flags
        {
            return flags;
        }

        [[nodiscard]] auto isReversed() const noexcept -> bool
        {
            return flags.reversed;
        }

        [[nodiscard]] auto isExtractable() const noexcept -> bool
        {
            return flags.extractable;
        }

        void setId(const SmallId new_id) noexcept
        {
            id = new_id;
        }

        void makeExtractable() noexcept
        {
            flags.extractable = true;
        }

        void hideFromParser() noexcept
        {
            flags.isHiddenFromParser = true;
        }

        auto reverse() noexcept -> void
        {
            flags.reversed = !flags.reversed;
        }

        auto setClosure(Closure new_repetition) noexcept -> void
        {
            closure = new_repetition;
        }

        [[nodiscard]] auto canBeOptimized() const noexcept -> bool
        {
            const auto fromZeroAccuracies = closure.from == 0;
            return !isReversed() && fromZeroAccuracies && empty();
        }

        [[nodiscard]] auto getId() const noexcept -> SmallId
        {
            return id;
        }

        static auto
            alwaysRecognizedSuggestion(const TextIterator &text_iterator, bool condition) -> void;

        static auto
            neverRecognizedSuggestion(const TextIterator &text_iterator, bool condition) -> void;

        [[nodiscard]] auto scan(ForkedGenerator text_iterator) const -> ScanResult;

        [[nodiscard]] virtual auto empty() const noexcept -> bool = 0;

        [[nodiscard]] virtual auto
            scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult = 0;
    };

    class RuleBlock : public std::unique_ptr<RuleBlockInterface>
    {
    public:
        using std::unique_ptr<RuleBlockInterface>::unique_ptr;

        explicit RuleBlock(std::unique_ptr<RuleBlockInterface> block)
          : unique_ptr(std::move(block))
        {}

        template<std::derived_from<RuleBlockInterface> T>
        explicit RuleBlock(T dot_item)
          : unique_ptr{std::make_unique<T>(std::move(dot_item))}
        {}
    };
}// namespace ccl::lexer::rule

namespace ccl::lexer
{
    using AnyPlaceItems = rule::RuleBlockInterface::AnyPlaceItems;
}

#endif /* CCL_PROJECT_RULE_INTERFACE_HPP */
