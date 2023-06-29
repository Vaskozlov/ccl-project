#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <ccl/lex/dot_item/container.hpp>
#include <ccl/lex/dot_item/sequence.hpp>
#include <ccl/lex/dot_item/union.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

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
        return !isEoF(ruleIterator.next());
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
            startLogicalOperator(LogicalOperation::AND);
            break;

        case U'|':
            startLogicalOperator(LogicalOperation::OR);
            break;

        default:
            throwUndefinedAction();
        }
    }

    auto Container::RuleParser::startLogicalOperator(LogicalOperation type) -> void
    {
        checkThereIsLhsItem();
        logicalOperation = type;

        constructedLhs = std::move(items.back());
        items.pop_back();
    }

    auto Container::RuleParser::tryToFinishLogicalOperation() -> void
    {
        if (constructedLhs.has_value() && !rhsItemConstructed) {
            rhsItemConstructed = true;
        } else if (constructedLhs.has_value() && rhsItemConstructed) {
            emplaceItem(constructLogicalUnit());
            logicalOperation = LogicalOperation::NONE;
        }
    }

    auto Container::RuleParser::constructLogicalUnit() -> LogicalUnit
    {
        auto rhs = std::move(items.back());
        items.pop_back();

        return LogicalUnit{
            std::move(constructedLhs.value()), std::move(rhs), logicalOperation, getId()};
    }

    auto Container::RuleParser::constructNewSequence() -> Sequence
    {
        tryToFinishLogicalOperation();

        return Sequence{Sequence::SequenceFlags{}, "\"", ruleIterator, getId()};
    }

    auto Container::RuleParser::constructNewUnion() -> Union
    {
        tryToFinishLogicalOperation();

        return Union{ruleIterator, getId()};
    }

    // NOLINTNEXTLINE (recursive function)
    auto Container::RuleParser::constructNewContainer() -> Container
    {
        tryToFinishLogicalOperation();

        auto text = ruleIterator.getRemainingWithCurrent();
        const auto *saved_end = ruleIterator.getEnd();
        auto bracket_index = findContainerEnd(text);

        ruleIterator.setEnd(text.begin() + bracket_index);

        auto new_container =
            Container{ruleIterator, specialItems, getId(), false, container.isSpecial()};
        ruleIterator.setEnd(saved_end);

        return new_container;
    }

    auto Container::RuleParser::finishPreviousItemInitialization() -> void
    {
        if (items.empty()) {
            return;
        }

        const auto &item = items.back();
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

    CCL_INLINE auto Container::RuleParser::checkId() const -> void
    {
        if (ReservedTokenType(getId()) == ReservedTokenType::BAD_TOKEN ||
            ReservedTokenType(getId()) == ReservedTokenType::EOI) {
            throw UnrecoverableError{
                "reserved token type (0 and 1 are reserved for EOI and BAD TOKEN)"};
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

        const auto postfixes_correct =
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
            tryToFinishLogicalOperation();
            return;
        }

        finishPreviousItemInitialization();
        DotItemConcept::neverRecognizedSuggestion(ruleIterator, items.empty() && !isReversed());
        DotItemConcept::alwaysRecognizedSuggestion(ruleIterator, items.empty() && isReversed());
    }

    auto Container::RuleParser::findContainerEnd(string_view repr) -> size_t
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

        auto &last_item = items.back();

        if (last_item->hasPrefix()) {
            throwUnableToApply("item already has prefix modifier");
        }

        if (last_item->hasPostfix()) {
            throwUnableToApply("item already has postfix modifier");
        }
    }

    auto Container::RuleParser::throwUnableToApply(string_view reason, string_view suggestion)
        -> void
    {
        auto message = fmt::format("unable to apply: {}", reason);

        ruleIterator.throwCriticalError(AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto Container::RuleParser::throwUndefinedAction() -> void
    {
        constexpr static auto message = "undefined action"_sv;
        constexpr static auto suggestion =
            "use `!` to declare special symbol, `\"` for string, `[` for unions, `(` for "
            "containers "_sv;

        ruleIterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto DotItemConcept::SpecialItems::checkForSpecial(const ForkedGenerator &text_iterator) const
        -> bool
    {
        return std::any_of(
            specialItems.cbegin(),
            specialItems.cend(),
            [&text_iterator](const Container &special_item) {
                return special_item.scan(text_iterator) != std::nullopt;
            });
    }

    auto DotItemConcept::SpecialItems::specialScan(TextIterator &text_iterator, Token &token) const
        -> bool
    {
        return std::any_of(
            specialItems.cbegin(),
            specialItems.cend(),
            [&text_iterator, &token](const Container &special_item) {
                return special_item.beginScan(text_iterator, token, ScanningType::SPECIAL);
            });
    }
}// namespace ccl::lex::dot_item
