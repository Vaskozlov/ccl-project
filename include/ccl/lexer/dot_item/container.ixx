module;
#include <ccl/defines.hpp>
export module ccl.lexer.dot_item:container;

export import isl;
export import ccl.text;
export import ccl.lexer.dot_item.core;
export import ccl.lexer.dot_item.binary_expression;
export import :sequence;
export import :union_item;

export namespace ccl::lexer::dot_item
{
    enum struct ScanningType : Id
    {
        MAIN_SCAN,
        BASIC,
        SPECIAL,
        CHECK
    };

    class Container;

    class SpecialItems
    {
    public:
        std::vector<Container> specialItems;

        [[nodiscard]] auto
            specialScan(text::TextIterator &text_iterator, Token &token) const -> bool;
        [[nodiscard]] auto
            checkForSpecial(const DotItemConcept::ForkedGenerator &text_iterator) const -> bool;
    };

    class Container final : public DotItemConcept
    {
        using DotItemConcept::canBeOptimized;
        using typename DotItemConcept::TextIterator;
        using DotItemsStorage = std::vector<DotItem>;

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
        [[nodiscard]] Container(
            TextIterator &rule_iterator, SpecialItems &special_items, Id item_id,
            bool main_item = false, bool is_special = false);

        [[nodiscard]] Container(
            const TextIterator &rule_iterator, SpecialItems &special_items, Id item_id,
            bool main_item = false, bool is_special = false);

        [[nodiscard]] auto beginScan(
            TextIterator &text_iterator, Token &token,
            ScanningType special_scan = ScanningType::BASIC) const -> bool;

        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> std::optional<std::size_t> override;

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

        [[nodiscard]] auto isSpecial() const noexcept -> bool
        {
            return flags.isSpecial;
        }

        [[nodiscard]] auto getItems() const noexcept CCL_LIFETIMEBOUND -> const DotItemsStorage &
        {
            return items;
        }

    private:
        auto parseRule(TextIterator &rule_iterator) -> void;

        [[nodiscard]] auto failedToEndItem(const ForkedGenerator &text_iterator) const -> bool;

        static auto addPrefixOrPostfix(
            const DotItemConcept *item, Token &token, isl::string_view repr) -> void;
    };

    class Container::RuleParser
    {
        Container &container;                              // NOLINT
        TextIterator &ruleIterator;                        // NOLINT
        DotItemsStorage &items{container.items};           // NOLINT
        SpecialItems &specialItems{container.specialItems};// NOLINT
        std::optional<DotItem> constructedLhs{std::nullopt};
        BinaryOperator binaryOperator{};
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

        auto startBinaryExpression(BinaryOperator type) -> void;

        auto tryToFinishBinaryExpression() -> void;

        [[nodiscard]] auto hasMovedToTheNextChar() -> bool;

        [[nodiscard]] auto constructBinaryExpression() -> DotItem;

        [[nodiscard]] auto constructNewSequence() -> Sequence;

        [[nodiscard]] auto constructNewUnion() -> Union;

        [[nodiscard]] auto constructNewContainer() -> Container;

        template<std::derived_from<DotItemConcept> T>
        auto emplaceItem(T item) -> void;

        auto emplaceItem(DotItem item) -> void;

        auto finishPreviousItemInitialization() -> void;

        auto addPrefixPostfix() -> void;

        auto addRepetition(Repetition new_repetition) -> void;

        auto makeSpecial() -> void;

        auto reverseLastItem() -> void;

        auto postCreationCheck() -> void;

        [[nodiscard]] auto findContainerEnd(isl::string_view repr) -> std::size_t;

        auto checkThereIsLhsItem() -> void;

        auto checkAbilityToCreatePrefixPostfix() -> void;

        [[noreturn]] auto
            throwUnableToApply(isl::string_view reason, isl::string_view suggestion = {}) -> void;

        [[noreturn]] auto throwUndefinedAction() -> void;
    };
}// namespace ccl::lexer::dot_item
