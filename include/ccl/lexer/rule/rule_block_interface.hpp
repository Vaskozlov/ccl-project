#ifndef CCL_PROJECT_RULE_INTERFACE_HPP
#define CCL_PROJECT_RULE_INTERFACE_HPP

#include <ccl/lexer/parsing_result.hpp>
#include <ccl/lexer/rule/closure.hpp>
#include <ccl/lexer/scan_result.hpp>
#include <ccl/lexer/token.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/text/text_iterator.hpp>
#include <isl/memory.hpp>

namespace ccl::lexer::rule
{
    class Container;

    class RuleBlockInterface
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
            bool isHiddenFromParser{};
        };

    private:
        Closure closure{1, 1};
        Id id{};
        Flags flags;

    public:
        [[nodiscard]] explicit RuleBlockInterface(Id item_id)
          : id{item_id}
        {}

        [[nodiscard]] RuleBlockInterface(Id item_id, Flags item_flags)
          : id{item_id}
          , flags{item_flags}
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

        [[nodiscard]] auto hasPrefix() const noexcept -> bool
        {
            return flags.isPrefix;
        }

        [[nodiscard]] auto hasPostfix() const noexcept -> bool
        {
            return flags.isPostfix;
        }

        void setId(Id new_id) noexcept
        {
            id = new_id;
        }

        void setPrefix() noexcept
        {
            flags.isPrefix = true;
        }

        void setPostfix() noexcept
        {
            flags.isPostfix = true;
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

    class RuleBlock : public isl::UniquePtr<RuleBlockInterface>
    {
    public:
        using isl::UniquePtr<RuleBlockInterface>::UniquePtr;

        explicit constexpr RuleBlock(isl::UniquePtr<RuleBlockInterface> block)
          : isl::UniquePtr<RuleBlockInterface>::UniquePtr(std::move(block))
        {}

        template<std::derived_from<RuleBlockInterface> T>
        explicit constexpr RuleBlock(T dot_item)
          : isl::UniquePtr<RuleBlockInterface>::UniquePtr(isl::makeUnique<T>(std::move(dot_item)))
        {}
    };
}// namespace ccl::lexer::rule

namespace ccl::lexer
{
    using AnyPlaceItems = rule::RuleBlockInterface::AnyPlaceItems;
}

#endif /* CCL_PROJECT_RULE_INTERFACE_HPP */
