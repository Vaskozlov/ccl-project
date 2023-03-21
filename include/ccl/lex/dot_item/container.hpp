#ifndef CCL_PROJECT_CONTAINER_HPP
#define CCL_PROJECT_CONTAINER_HPP

#include <ccl/lex/dot_item/item_concept.hpp>
#include <ccl/lex/dot_item/logical_unit.hpp>
#include <ccl/lex/dot_item/repetition.hpp>
#include <ccl/lex/dot_item/sequence.hpp>
#include <ccl/lex/dot_item/union.hpp>

namespace ccl::lex::dot_item
{
    enum struct ScanningType : Id
    {
        MAIN_SCAN,
        BASIC,
        SPECIAL,
        CHECK
    };

    class Container;

    class Container final : public DotItemConcept
    {
        using DotItemConcept::canBeOptimized;
        using typename DotItemConcept::TextIterator;
        using DotItemsStorage = Vector<DotItem>;

        class RuleParser;

        struct ContainerFlags
        {
            bool isMain = false;
            bool isSpecial = false;
        };

        DotItemsStorage items{};
        SpecialItems &specialItems;
        ContainerFlags flags{};

    public:
        Container(
            TextIterator &rule_iterator, SpecialItems &special_items, Id item_id,
            bool main_item = false, bool is_special = false);

        Container(
            const TextIterator &rule_iterator, SpecialItems &special_items, Id item_id,
            bool main_item = false, bool is_special = false);

        auto beginScan(
            TextIterator &text_iterator, Token &token,
            ScanningType special_scan = ScanningType::BASIC) const -> bool;

        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> size_t final;

        [[nodiscard]] auto operator==(const Container &other) const noexcept -> bool
        {
            return getId() == other.getId();
        }

        [[nodiscard]] auto operator<=>(const Container &other) const noexcept -> std::weak_ordering
        {
            return getId() <=> other.getId();
        }

        [[nodiscard]] auto empty() const noexcept -> bool final
        {
            return items.empty();
        }

        [[nodiscard]] auto isSpecial() const noexcept -> bool
        {
            return flags.isSpecial;
        }

        [[nodiscard]] auto getItems() const noexcept -> const DotItemsStorage &
        {
            return items;
        }

    private:
        auto parseRule(TextIterator &rule_iterator) -> void;

        [[nodiscard]] auto failedToEndItem(const ForkedGenerator &text_iterator) const -> bool;

        static auto
            addPrefixOrPostfix(const DotItemConcept *item, Token &token, const string_view &repr)
                -> void;
    };

    class Container::RuleParser
    {
        Container &container;                              // NOLINT
        TextIterator &ruleIterator;                        // NOLINT
        DotItemsStorage &items{container.items};           // NOLINT
        SpecialItems &specialItems{container.specialItems};// NOLINT
        Optional<DotItem> constructedLhs{std::nullopt};
        LogicalOperation logicalOperation{};
        bool rhsItemConstructed{false};

    public:
        RuleParser(Container &target_container, TextIterator &text_iterator);

    private:
        [[nodiscard]] CCL_INLINE auto getId() const noexcept -> Id
        {
            return container.getId();
        }

        [[nodiscard]] CCL_INLINE auto isReversed() const noexcept -> bool
        {
            return container.isReversed();
        }

        auto checkId() const -> void;

        auto recognizeAction() -> void;

        auto startLogicalOperator(LogicalOperation type) -> void;

        auto tryToFinishLogicalOperation() -> void;

        [[nodiscard]] auto hasMovedToTheNextChar() -> bool;

        [[nodiscard]] auto constructLogicalUnit() -> LogicalUnit;

        [[nodiscard]] auto constructNewSequence() -> Sequence;

        [[nodiscard]] auto constructNewUnion() -> Union;

        [[nodiscard]] auto constructNewContainer() -> Container;

        template<std::derived_from<DotItemConcept> T>
        auto emplaceItem(T item) -> void
        {
            if (!item.canBeOptimized()) {
                finishPreviousItemInitialization();
                items.emplace_back(std::move(item));
            }
        }

        auto finishPreviousItemInitialization() -> void;

        auto addPrefixPostfix() -> void;

        auto addRepetition(Repetition new_repetition) -> void;

        auto makeSpecial() -> void;

        auto reverseLastItem() -> void;

        auto postCreationCheck() -> void;

        [[nodiscard]] auto findContainerEnd(string_view repr) -> size_t;

        auto checkThereIsLhsItem() -> void;

        auto checkAbilityToCreatePrefixPostfix() -> void;

        auto throwUnableToApply(string_view reason, string_view suggestion = {}) -> void;

        auto throwUndefinedAction() -> void;
    };

    class DotItemConcept::SpecialItems
    {
    public:
        Vector<Container> specialItems;

        [[nodiscard]] auto specialScan(TextIterator &text_iterator, Token &token) const -> bool;
        [[nodiscard]] auto checkForSpecial(const ForkedGenerator &text_iterator) const -> bool;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_CONTAINER_HPP */
