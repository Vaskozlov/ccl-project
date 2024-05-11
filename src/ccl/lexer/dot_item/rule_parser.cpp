#include <ccl/lexer/analyzer_generator/analyzer_generator.hpp>
#include <ccl/lexer/dot_item/container.hpp>
#include <ccl/lexer/dot_item/sequence.hpp>
#include <ccl/lexer/dot_item/union.hpp>

namespace ccl::lexer::dot_item
{
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
        return !isEoF(ruleIterator.advance());
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
            addRepetition(Closure{0, Closure::max()});
            break;

        case U'+':
            addRepetition(Closure{1, Closure::max()});
            break;

        case U'?':
            addRepetition(Closure{0, 1});
            break;

        case U'{':
            addRepetition(Closure{ruleIterator});
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

        isl::unreachable();
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
            Container{ruleIterator, anyPlaceItems, getId(), false, container.isAnyPlaceRule()};
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

    auto Container::RuleParser::addRepetition(Closure new_repetition) -> void
    {
        if (items.empty()) {
            throwUnableToApply("no items found to set repetition");
        }

        auto &last_item = items.back();

        if (last_item->getRepetition() != Closure{1, 1}) {
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

    CCL_INLINE auto Container::RuleParser::checkId() const -> void
    {
        if (getId() <= ReservedTokenMaxValue) {
            throw UnrecoverableError{
                fmt::format("tokens' ids from 0 to {} are reserved", ReservedTokenMaxValue)};
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
        const auto postfix_elem = std::ranges::find_if(items, [](const DotItem &elem) {
            return elem->hasPostfix();
        });

        const bool postfixes_correct =
            std::ranges::all_of(postfix_elem, items.cend(), [](const DotItem &elem) {
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

        ruleIterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, "unterminated dot item");
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

        const auto &last_item = items.back();

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
        auto message = fmt::format("unable to apply: {}", reason);

        ruleIterator.throwCriticalError(AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto Container::RuleParser::throwUndefinedAction() -> void
    {
        using namespace isl::string_view_literals;

        constexpr static auto message = "undefined action"_sv;
        constexpr static auto suggestion =
            "use `!` to declare special symbol, `\"` for string, `[` for unions, `(` for "
            "containers "_sv;

        ruleIterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto DotItemConcept::AnyPlaceItems::checkForSpecial(const ForkedGenerator &text_iterator) const
        -> bool
    {
        return std::ranges::any_of(items, [&text_iterator](const Container &special_item) {
            return special_item.scan(text_iterator).isSuccess();
        });
    }

    auto DotItemConcept::AnyPlaceItems::isSuccessfulScan(TextIterator &text_iterator, Token &token)
        const -> bool
    {
        return std::ranges::any_of(items, [&text_iterator, &token](const Container &special_item) {
            return special_item.beginScan(text_iterator, token, ScanType::SPECIAL);
        });
    }
}// namespace ccl::lexer::dot_item
