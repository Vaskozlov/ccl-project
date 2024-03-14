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

        class RuleParser
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
            [[nodiscard]] auto getId() const noexcept -> Id
            {
                return container.getId();
            }

            [[nodiscard]] auto isReversed() const noexcept -> bool
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

            [[noreturn]] auto throwUnableToApply(
                isl::string_view reason, isl::string_view suggestion = {}) -> void;

            [[noreturn]] auto throwUndefinedAction() -> void;
        };

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
            bool main_item = false, bool is_special = false)
          : DotItemConcept{item_id}
          , specialItems{special_items}
          , flags{.isMain = main_item, .isSpecial = is_special}
        {
            parseRule(rule_iterator);
        }

        [[nodiscard]] Container(
            const TextIterator &rule_iterator, SpecialItems &special_items, Id item_id,
            bool main_item = false, bool is_special = false)
          : DotItemConcept{item_id}
          , specialItems{special_items}
          , flags{.isMain = main_item, .isSpecial = is_special}
        {
            auto text_iterator_copy = rule_iterator;
            parseRule(text_iterator_copy);
        }

        [[nodiscard]] auto beginScan(
            TextIterator &text_iterator, Token &token,
            ScanningType special_scan = ScanningType::BASIC) const -> bool
        {
            auto totally_skipped = 0ZU;
            auto local_iterator = text_iterator.fork();

            token.clear(getId());

            for (const DotItem &item : items) {
                auto chars_to_skip = item->scan(local_iterator);
                const bool has_moved = chars_to_skip.has_value();
                const bool succeed_as_reversed = !has_moved && isReversed();

                if (succeed_as_reversed) {
                    chars_to_skip = isl::utf8::size(local_iterator.getNextCarriageValue());
                } else if (!has_moved) {
                    return false;
                }

                auto prefix_or_postfix_repr =
                    isl::string_view{local_iterator.getRemainingAsCarriage(), *chars_to_skip};

                addPrefixOrPostfix(item.get(), token, prefix_or_postfix_repr);

                totally_skipped += *chars_to_skip;
                local_iterator.skip(*chars_to_skip);
            }

            if (special_scan == ScanningType::BASIC && failedToEndItem(local_iterator))
                [[unlikely]] {
                return false;
            } else if (special_scan == ScanningType::CHECK) {
                return true;
            }

            token.finishInitialization(text_iterator, totally_skipped);
            return true;
        }

        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> std::optional<std::size_t> override
        {
            auto totally_skipped = 0ZU;
            auto local_iterator = text_iterator;

            for (const DotItem &item : items) {
                auto scan_result = item->scan(local_iterator);

                if (!scan_result.has_value()) {
                    return std::nullopt;
                }

                totally_skipped += *scan_result;
                local_iterator.skip(*scan_result);
            }

            return totally_skipped;
        }

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

        [[nodiscard]] auto getItems() const noexcept -> const DotItemsStorage &
        {
            return items;
        }

    private:
        auto parseRule(TextIterator &rule_iterator) -> void
        {
            RuleParser{*this, rule_iterator};
        }

        [[nodiscard]] auto failedToEndItem(const ForkedGenerator &text_iterator) const -> bool
        {
            return !(
                text::isLayoutOrEoF(text_iterator.getNextCarriageValue()) ||
                specialItems.checkForSpecial(text_iterator));
        }

        static auto addPrefixOrPostfix(
            const DotItemConcept *item, Token &token, isl::string_view repr) -> void
        {
            if (repr.empty()) [[unlikely]] {
                return;
            }

            if (item->hasPrefix()) [[unlikely]] {
                token.addPrefix(repr);
            } else if (item->hasPostfix()) [[unlikely]] {
                token.addPostfix(repr);
            }
        }
    };

    Container::RuleParser::RuleParser(Container &target_container, TextIterator &text_iterator)
      : container{target_container}
      , ruleIterator{text_iterator}
    {
        checkId();

        ruleIterator.moveToCleanChar();

        while (hasMovedToTheNextChar()) {
            recognizeAction();
            ruleIterator.moveToCleanChar();
        }

        postCreationCheck();
    }

    auto Container::RuleParser::hasMovedToTheNextChar() -> bool
    {
        return !text::isEoF(ruleIterator.advance());
    }

    // NOLINTNEXTLINE recursive function
    auto Container::RuleParser::recognizeAction() -> void
    {
        switch (ruleIterator.getCurrentChar()) {
        case '!':
            makeSpecial();
            break;

        case U'[':
            emplaceItem(constructNewUnion());
            break;

        case U'\"':
            emplaceItem(constructNewSequence());
            break;

        case U'(':
            emplaceItem(constructNewContainer());
            break;

        case U'*':
            addRepetition(Repetition::star());
            break;

        case U'+':
            addRepetition(Repetition::plus());
            break;

        case U'?':
            addRepetition(Repetition::question());
            break;

        case U'{':
            addRepetition(Repetition{ruleIterator});
            break;

        case U'^':
            reverseLastItem();
            break;

        case U'p':
            addPrefixPostfix();
            break;

        case U'&':
            startBinaryExpression(BinaryOperator::AND);
            break;

        case U'|':
            startBinaryExpression(BinaryOperator::OR);
            break;

        default:
            throwUndefinedAction();
        }
    }

    auto Container::RuleParser::startBinaryExpression(BinaryOperator type) -> void
    {
        tryToFinishBinaryExpression();

        checkThereIsLhsItem();
        binaryOperator = type;

        constructedLhs = std::move(items.back());
        items.pop_back();
    }

    auto Container::RuleParser::tryToFinishBinaryExpression() -> void
    {
        if (constructedLhs.has_value() && !rhsItemConstructed) {
            rhsItemConstructed = true;
        } else if (constructedLhs.has_value() && rhsItemConstructed) {
            emplaceItem(constructBinaryExpression());
            rhsItemConstructed = false;
            binaryOperator = BinaryOperator::NONE;
        }
    }

    auto Container::RuleParser::constructBinaryExpression() -> DotItem
    {
        auto rhs = std::move(items.back());
        items.pop_back();

        if (binaryOperator == BinaryOperator::AND) {
            return DotItem{
                BinaryOperationAnd(std::move(constructedLhs.value()), std::move(rhs), getId())};
        }

        if (binaryOperator == BinaryOperator::OR) {
            return DotItem{
                BinaryOperationOr(std::move(constructedLhs.value()), std::move(rhs), getId())};
        }

        std::unreachable();
    }

    auto Container::RuleParser::constructNewSequence() -> Sequence
    {
        tryToFinishBinaryExpression();

        return Sequence{Sequence::SequenceFlags{}, "\"", ruleIterator, getId()};
    }

    auto Container::RuleParser::constructNewUnion() -> Union
    {
        tryToFinishBinaryExpression();

        return Union{ruleIterator, getId()};
    }

    // NOLINTNEXTLINE (recursive function)
    auto Container::RuleParser::constructNewContainer() -> Container
    {
        tryToFinishBinaryExpression();

        auto text = ruleIterator.getRemainingWithCurrent();
        const auto *saved_end = ruleIterator.getEnd();
        auto bracket_index = findContainerEnd(text);

        ruleIterator.setEnd(text.begin() + bracket_index);

        auto new_container =
            Container{ruleIterator, specialItems, getId(), false, container.isSpecial()};
        ruleIterator.setEnd(saved_end);

        return new_container;
    }

    template<std::derived_from<DotItemConcept> T>
    auto Container::RuleParser::emplaceItem(T item) -> void
    {
        if (!item.canBeOptimized()) {
            finishPreviousItemInitialization();
            items.emplace_back(std::move(item));
        }
    }

    auto Container::RuleParser::emplaceItem(DotItem item) -> void
    {
        if (!item->canBeOptimized()) {
            finishPreviousItemInitialization();
            items.emplace_back(std::move(item));
        }
    }

    auto Container::RuleParser::finishPreviousItemInitialization() -> void
    {
        if (items.empty()) {
            return;
        }

        const DotItem &item = items.back();
        auto item_repetition = item->getRepetition();

        neverRecognizedSuggestion(
            ruleIterator, item_repetition.to == 0 || (item->empty() && !item->isReversed()));

        alwaysRecognizedSuggestion(
            ruleIterator, item_repetition.to != 0 && item->empty() && item->isReversed());
    }


    auto Container::RuleParser::addPrefixPostfix() -> void
    {
        checkAbilityToCreatePrefixPostfix();

        auto &last_item = items.back();
        auto is_prefix = items.size() == 1 || items[items.size() - 2]->hasPrefix();

        if (is_prefix) {
            last_item->setPrefix();
        } else {
            last_item->setPostfix();
        }
    }

    auto Container::RuleParser::addRepetition(Repetition new_repetition) -> void
    {
        if (items.empty()) {
            throwUnableToApply("no items found to set repetition");
        }

        auto &last_item = items.back();

        if (last_item->getRepetition() != Repetition::basic()) {
            throwUnableToApply("item already has repetition");
        }

        last_item->setRepetition(new_repetition);
    }

    auto Container::RuleParser::makeSpecial() -> void
    {
        if (!items.empty()) {
            throwUnableToApply("special must be applied before anything else");
        }

        container.flags.isSpecial = true;
    }

    auto Container::RuleParser::checkId() const -> void
    {
        if (getId() <= ReservedTokenMaxValue) {
            throw UnrecoverableError{
                std::format("tokens' ids from 0 to {} are reserved", ReservedTokenMaxValue)};
        }
    }

    auto Container::RuleParser::reverseLastItem() -> void
    {
        if (items.empty()) {
            throwUnableToApply("no items to reverse");
        }

        auto &last_item = items.back();

        if (last_item->isReversed()) {
            throwUnableToApply("item already has reverse modifier");
        }

        last_item->reverse();
    }

    auto Container::RuleParser::postCreationCheck() -> void
    {
        const auto postfix_elem =
            std::find_if(items.cbegin(), items.cend(), [](const DotItem &elem) {
                return elem->hasPostfix();
            });

        const bool postfixes_correct =
            std::all_of(postfix_elem, items.cend(), [](const DotItem &elem) {
                return elem->hasPostfix();
            });

        if (!postfixes_correct) {
            throwUnableToApply("item without postfix modifier exists after items with it");
        }

        if (constructedLhs.has_value() && !rhsItemConstructed) {
            throwUnableToApply("no rhs items to apply operation");
        }

        if (constructedLhs.has_value() && rhsItemConstructed) {
            tryToFinishBinaryExpression();
            return;
        }

        finishPreviousItemInitialization();
        DotItemConcept::neverRecognizedSuggestion(ruleIterator, items.empty() && !isReversed());
        DotItemConcept::alwaysRecognizedSuggestion(ruleIterator, items.empty() && isReversed());
    }

    auto Container::RuleParser::findContainerEnd(isl::string_view repr) -> std::size_t
    {
        const auto closing_bracket_index = repr.findMatchingPair('(', ')');

        if (closing_bracket_index.has_value()) {
            return *closing_bracket_index;
        }

        ruleIterator.throwPanicError(
            text::AnalysisStage::LEXICAL_ANALYSIS, "unterminated dot item");
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto Container::RuleParser::checkThereIsLhsItem() -> void
    {
        if (items.empty()) [[unlikely]] {
            throwUnableToApply("no left hand side items to apply operation");
        }
    }

    auto Container::RuleParser::checkAbilityToCreatePrefixPostfix() -> void
    {
        if (!container.flags.isMain) {
            throwUnableToApply(
                "you are not allowed to create prefixes or postfixes inside other containers");
        }

        if (items.empty()) {
            throwUnableToApply("there are not any items to apply prefix/postfix");
        }

        auto &last_item = items.back();

        if (last_item->hasPrefix()) {
            throwUnableToApply("item already has prefix modifier");
        }

        if (last_item->hasPostfix()) {
            throwUnableToApply("item already has postfix modifier");
        }
    }

    auto Container::RuleParser::throwUnableToApply(
        isl::string_view reason,
        isl::string_view suggestion) -> void
    {
        auto message = std::format("unable to apply: {}", reason);

        ruleIterator.throwCriticalError(text::AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto Container::RuleParser::throwUndefinedAction() -> void
    {
        using namespace isl::string_view_literals;

        constexpr static auto message = "undefined action"_sv;
        constexpr static auto suggestion =
            "use `!` to declare special symbol, `\"` for string, `[` for unions, `(` for "
            "containers "_sv;

        ruleIterator.throwPanicError(text::AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto
        SpecialItems::checkForSpecial(const Container::ForkedGenerator &text_iterator) const -> bool
    {
        return std::any_of(
            specialItems.cbegin(),
            specialItems.cend(),
            [&text_iterator](const Container &special_item) {
                return special_item.scan(text_iterator) != std::nullopt;
            });
    }

    auto SpecialItems::specialScan(DotItemConcept::TextIterator &text_iterator, Token &token) const
        -> bool
    {
        return std::any_of(
            specialItems.cbegin(),
            specialItems.cend(),
            [&text_iterator, &token](const Container &special_item) {
                return special_item.beginScan(text_iterator, token, ScanningType::SPECIAL);
            });
    }
}// namespace ccl::lexer::dot_item
