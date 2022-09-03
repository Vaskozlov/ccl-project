#ifndef CCL_PROJECT_CONTAINER_HPP
#define CCL_PROJECT_CONTAINER_HPP

#include <boost/container/small_vector.hpp>
#include <ccl/lex/dot_item/basic_item.hpp>
#include <ccl/lex/dot_item/logical_unit.hpp>
#include <ccl/lex/dot_item/recurrence.hpp>
#include <stack>

namespace ccl::lex::dot_item
{
    class Container final : public BasicItem
    {
    private:
        using BasicItem::canBeOptimized;
        using BasicItem::recurrence;

        using typename BasicItem::TextIterator;
        using storage_t = boost::container::small_vector<std::unique_ptr<BasicItem>, 4>;

        struct RuleParser;

    public:
        Container(
            TextIterator &rule_iterator_, size_t id_, SpecialItems &special_items_,
            bool main_item_ = false)
          : BasicItem(special_items_, id_), main_item(main_item_)
        {
            parseRule(rule_iterator_);
        }

        Container(
            TextIterator &&rule_iterator_, size_t id_, SpecialItems &special_items_,
            bool main_item_ = true)
          : BasicItem(special_items_, id_), main_item(main_item_)
        {
            parseRule(rule_iterator_);
        }

        Container(
            const TextIterator &rule_iterator_, size_t id_, SpecialItems &special_items_,
            bool main_item_ = true)
          : BasicItem(special_items_, id_), main_item(main_item_)
        {
            auto rule_iterator = rule_iterator_;
            parseRule(rule_iterator);
        }

        [[nodiscard]] auto operator==(const Container &other) const noexcept
        {
            return id == other.id;
        }

        [[nodiscard]] auto operator<=>(const Container &other) const noexcept
        {
            return id <=> other.id;
        }

        [[nodiscard]] auto empty() const noexcept -> bool final
        {
            return items.empty();
        }

        [[nodiscard]] auto isSpecial() const noexcept -> bool
        {
            return is_special;
        }

        [[nodiscard]] auto getItems() const noexcept -> const storage_t &
        {
            return items;
        }

    private:
        [[nodiscard]] auto scanIteration(TextIterator &text_iterator, Token &token) const
            -> bool final;

        [[nodiscard]] auto
            scanItem(const BasicItem *item, TextIterator &text_iterator, Token &token) const
            -> bool;

        auto parseRule(TextIterator &rule_iterator) -> void;

        storage_t items{};
        bool main_item{};
        bool is_special{ false };
    };

    struct Container::RuleParser
    {
        RuleParser(Container &container_, TextIterator &rule_iterator_);

    private:
        [[nodiscard]] auto getId() const noexcept -> size_t
        {
            return container.id;
        }

        [[nodiscard]] auto isReversed() const noexcept -> bool
        {
            return container.reversed;
        }

        auto checkId() const -> void;

        auto recognizeAction() -> void;

        auto prepareForLogicalOperation(LogicalUnitType type) -> void;

        auto tryToFinishLogicalOperation() -> void;

        [[nodiscard]] auto hasMovedToTheNextChar() -> bool;

        [[nodiscard]] auto constructLogicalUnit() -> std::unique_ptr<BasicItem>;

        [[nodiscard]] auto constructNewSequence() -> std::unique_ptr<BasicItem>;

        [[nodiscard]] auto constructNewUnion() -> std::unique_ptr<BasicItem>;

        [[nodiscard]] auto constructNewContainer() -> std::unique_ptr<BasicItem>;

        auto emplaceItem(std::unique_ptr<BasicItem> &&item) -> void;

        auto addPrefixPostfix() -> void;

        auto addRecurrence(Recurrence new_recurrence) -> void;

        auto makeSpecial() -> void;

        auto reverseLastItem() -> void;

        auto postCreationCheck() -> void;

        auto findContainerEnd(string_view repr) -> size_t;

        auto checkThereIsLhsItem() -> void;

        auto checkAbilityToCreatePrefixPostfix() -> void;

        auto throwUnableToApply(string_view reason, string_view suggestion = {}) -> void;

        auto throwUndefinedAction() -> void;

        Container &container;
        TextIterator &rule_iterator;
        storage_t &items{ container.items };
        SpecialItems &special_items{ container.special_items };
        std::optional<std::unique_ptr<BasicItem>> reserved_lhs{ std::nullopt };
        LogicalUnitType logical_unit_type;
        bool rhs_item_constructed{ false };
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_CONTAINER_HPP */
