#ifndef CCL_PROJECT_CONTAINER_HPP
#define CCL_PROJECT_CONTAINER_HPP

#include <ccl/lexer/rule/binary_expression/binary_expression_base.hpp>
#include <ccl/lexer/rule/closure.hpp>
#include <ccl/lexer/rule/rule_block_interface.hpp>
#include <ccl/lexer/token.hpp>

namespace ccl::lexer
{
    class LexicalAnalyzer;
}

namespace ccl::lexer::rule
{
    enum struct ScanType : SmallId
    {
        BASIC,
        SPECIAL
    };

    class Container;

    class Container final : public RuleBlockInterface
    {
        using RuleBlockInterface::canBeOptimized;
        using RuleBlockInterface::TextIterator;
        using DotItemsStorage = std::vector<RuleBlock>;

        class RuleParser;

        struct ContainerFlags
        {
            bool isMain{};
            bool isAnyPlace{};
        };

        DotItemsStorage items;
        LexicalAnalyzer &lexicalAnalyzer;// NOLINT reference
        AnyPlaceItems &anyPlaceItems;    // NOLINT reference
        ContainerFlags flags;

    public:
        [[nodiscard]] Container(
            LexicalAnalyzer &lexical_analyzer, TextIterator &rule_iterator, bool main_item = false,
            bool is_special = false);

        [[nodiscard]] Container(
            LexicalAnalyzer &lexical_analyzer, const TextIterator &rule_iterator,
            bool main_item = false, bool is_special = false);

        [[nodiscard]] Container(
            LexicalAnalyzer &lexical_analyzer, bool main_item = false, bool is_special = false);

        auto addItem(RuleBlock new_item) -> void
        {
            items.emplace_back(std::move(new_item));
        }

        [[nodiscard]] auto beginScan(
            TextIterator &text_iterator, Token &token,
            ScanType special_scan = ScanType::BASIC) const -> bool;

        [[nodiscard]] auto
            scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult override;

        [[nodiscard]] auto operator==(const Container &other) const noexcept -> bool
        {
            return getId() == other.getId();
        }

        [[nodiscard]] auto operator<=>(const Container &other) const noexcept -> std::weak_ordering
        {
            return getId() <=> other.getId();
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return items.empty();
        }

        [[nodiscard]] auto isAnyPlaceItem() const noexcept -> bool
        {
            return flags.isAnyPlace;
        }

        auto makeAsAnyPlaceItem() -> void
        {
            flags.isAnyPlace = true;
        }

        [[nodiscard]] auto getItems() const noexcept CCL_LIFETIMEBOUND -> const DotItemsStorage &
        {
            return items;
        }

    private:
        auto parseRule(TextIterator &rule_iterator) -> void;

        [[nodiscard]] auto
            itemSuccessfullyEnded(const ForkedGenerator &text_iterator) const -> bool;

        static auto addPrefixOrPostfix(
            const RuleBlockInterface *item, Token &token, isl::string_view repr) -> void;
    };

    class Container::RuleParser
    {
        Container &container;                                 // NOLINT reference
        TextIterator &ruleIterator;                           // NOLINT reference
        DotItemsStorage &items{container.items};              // NOLINT reference
        AnyPlaceItems &anyPlaceItems{container.anyPlaceItems};// NOLINT reference
        LexicalAnalyzer &lexicalAnalyzer;                     // NOLINT reference
        std::optional<RuleBlock> constructedLhs{std::nullopt};
        BinaryOperator binaryOperator{};
        bool rhsItemConstructed{};

    public:
        RuleParser(Container &target_container, TextIterator &text_iterator);

    private:
        [[nodiscard]] auto getId() const noexcept -> SmallId
        {
            return container.getId();
        }

        [[nodiscard]] auto isReversed() const noexcept -> bool
        {
            return container.isReversed();
        }

        auto recognizeAction() -> void;

        auto startBinaryExpression(BinaryOperator type) -> void;

        auto tryToFinishBinaryExpression() -> void;

        [[nodiscard]] auto hasMovedToTheNextChar() -> bool;

        [[nodiscard]] auto constructBinaryExpression() -> RuleBlock;

        [[nodiscard]] auto constructNewSequence() -> RuleBlock;

        [[nodiscard]] auto constructNewRuleReference() -> RuleBlock;

        [[nodiscard]] auto constructNewUnion() -> RuleBlock;

        [[nodiscard]] auto constructNewContainer() -> RuleBlock;

        auto emplaceItem(std::derived_from<RuleBlockInterface> auto item) -> void;

        auto emplaceItem(RuleBlock item) -> void;

        auto completePreviousItemInitialization() -> void;

        auto hideFromParser() -> void;

        auto addPrefixPostfix() -> void;

        auto addRepetition(Closure new_repetition) -> void;

        auto makeSpecial() -> void;

        auto reverseLastItem() -> void;

        auto postCreationCheck() -> void;

        [[nodiscard]] auto findContainerEnd(isl::string_view repr) -> std::size_t;

        auto checkThereIsLhsItem() -> void;

        auto checkAbilityToCreatePrefixOrPostfix() -> void;

        [[noreturn]] auto
            throwUnableToApply(isl::string_view reason, isl::string_view suggestion = {}) -> void;

        [[noreturn]] auto throwUndefinedAction() -> void;
    };

    class RuleBlockInterface::AnyPlaceItems
    {
    public:
        std::vector<std::unique_ptr<Container>> items;

        [[nodiscard]] auto
            isSuccessfulScan(TextIterator &text_iterator, Token &token) const -> bool;

        [[nodiscard]] auto checkForSpecial(const ForkedGenerator &text_iterator) const -> bool;
    };
}// namespace ccl::lexer::rule

#endif /* CCL_PROJECT_CONTAINER_HPP */
