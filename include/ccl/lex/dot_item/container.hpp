#ifndef CCL_PROJECT_CONTAINER_HPP
#define CCL_PROJECT_CONTAINER_HPP

#include <boost/container/small_vector.hpp>
#include <ccl/lex/dot_item/basic_item.hpp>
#include <ccl/lex/dot_item/logical_unit.hpp>
#include <ccl/lex/dot_item/repetition.hpp>
#include <stack>

namespace ccl::lex::dot_item
{
    CCL_ENUM(ScanningType, u16, MAIN_SCAN, BASIC, SPECIAL, CHECK);// NOLINT

    class Container final : public BasicItem
    {
        using BasicItem::canBeOptimized;
        using BasicItem::repetition;

        using typename BasicItem::TextIterator;

        using ForkedGen = typename TextIterator::ForkedTextIterator;
        using storage_t = boost::container::small_vector<UniquePtr<BasicItem>, 4>;

        class RuleParser;

        struct ContainerFlags
        {
            bool is_main : 1 = false;
            bool is_special : 1 = false;
        };

        storage_t items{};
        SpecialItems &specialItems;
        ContainerFlags flags{};

    public:
        Container(
            TextIterator &rule_iterator_, SpecialItems &special_items_, size_t id_,
            bool main_item_ = false, bool is_special_ = false);

        Container(
            TextIterator &&rule_iterator_, SpecialItems &special_items_, size_t id_,
            bool main_item_ = false, bool is_special_ = false);

        Container(
            const TextIterator &rule_iterator_, SpecialItems &special_items_, size_t id_,
            bool main_item_ = false, bool is_special_ = false);

        auto beginScan(
            TextIterator &text_iterator, Token &token,
            ScanningType special_scan = ScanningType::BASIC) const -> bool;

        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> size_t final;

        [[nodiscard]] auto operator==(const Container &other) const noexcept
        {
            return id == other.id;
        }

        [[nodiscard]] auto operator<=>(const Container &other) const noexcept -> std::weak_ordering
        {
            return id <=> other.id;
        }

        [[nodiscard]] auto empty() const noexcept -> bool final
        {
            return items.empty();
        }

        [[nodiscard]] auto isSpecial() const noexcept -> bool
        {
            return flags.is_special;
        }

        [[nodiscard]] auto getItems() const noexcept -> const storage_t &
        {
            return items;
        }

    private:
        auto parseRule(TextIterator &rule_iterator) -> void;

        [[nodiscard]] auto failedToEndItem(const ForkedGenerator &text_iterator) const -> bool;
    };

    class Container::RuleParser
    {
        Container &container;
        TextIterator &ruleIterator;
        storage_t &items{ container.items };
        SpecialItems &specialItems{ container.specialItems };
        std::optional<UniquePtr<BasicItem>> constructedLhs{ std::nullopt };
        LogicalOperation logicalOperation{};
        bool rhsItemConstructed{ false };

    public:
        RuleParser(Container &container_, TextIterator &rule_iterator_);

    private:
        [[nodiscard]] auto getId() const noexcept -> size_t
        {
            return container.id;
        }

        [[nodiscard]] auto isReversed() const noexcept -> bool
        {
            return container.isReversed();
        }

        auto checkId() const -> void;

        auto recognizeAction() -> void;

        auto startLogicalOperationConstruction(LogicalOperation type) -> void;

        auto tryToFinishLogicalOperation() -> void;

        [[nodiscard]] auto hasMovedToTheNextChar() -> bool;

        [[nodiscard]] auto constructLogicalUnit() -> UniquePtr<BasicItem>;

        [[nodiscard]] auto constructNewSequence() -> UniquePtr<BasicItem>;

        [[nodiscard]] auto constructNewUnion() -> UniquePtr<BasicItem>;

        [[nodiscard]] auto constructNewContainer() -> UniquePtr<BasicItem>;

        auto emplaceItem(UniquePtr<BasicItem> &&item) -> void;

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

    class BasicItem::SpecialItems
    {
    public:
        Vector<Container> special_items;

        [[nodiscard]] auto specialScan(TextIterator &text_iterator, Token &token) const -> bool;
        [[nodiscard]] auto checkForSpecial(const ForkedGenerator &text_iterator) const -> bool;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_CONTAINER_HPP */
