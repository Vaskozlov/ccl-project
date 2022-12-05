#include "ccl/lex/analyzer_generator/analyzer_generator.hpp"
#include "ccl/lex/token.hpp"
#include <ccl/lex/dot_item/container.hpp>
#include <ccl/lex/dot_item/sequence.hpp>
#include <ccl/lex/dot_item/union.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    Container::RuleParser::RuleParser(Container &container_, TextIterator &rule_iterator_)
      : container{container_}
      , ruleIterator{rule_iterator_}
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
            break;
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

    auto Container::RuleParser::constructLogicalUnit() -> UniquePtr<BasicItem>
    {
        auto rhs = std::move(items.back());
        items.pop_back();

        return makeUnique<BasicItem, LogicalUnit>(
            std::move(constructedLhs.value()), std::move(rhs), logicalOperation, getId());
    }

    auto Container::RuleParser::constructNewSequence() -> UniquePtr<BasicItem>
    {
        tryToFinishLogicalOperation();

        return makeUnique<BasicItem, Sequence>(
            Sequence::SequenceFlags{}, "\"", ruleIterator, getId());
    }

    auto Container::RuleParser::constructNewUnion() -> UniquePtr<BasicItem>
    {
        tryToFinishLogicalOperation();

        return makeUnique<BasicItem, Union>(ruleIterator, getId());
    }

    // NOLINTNEXTLINE (recursive function)
    auto Container::RuleParser::constructNewContainer() -> UniquePtr<BasicItem>
    {
        tryToFinishLogicalOperation();

        auto text = ruleIterator.getRemainingWithCurrent();
        const auto *saved_end = ruleIterator.getEnd();
        auto bracket_index = findContainerEnd(text);

        ruleIterator.setEnd(text.begin() + bracket_index);

        auto new_container = makeUnique<BasicItem, Container>(
            ruleIterator, specialItems, getId(), false, container.isSpecial());
        ruleIterator.setEnd(saved_end);

        return new_container;
    }

    auto Container::RuleParser::emplaceItem(UniquePtr<BasicItem> item) -> void
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

        const auto *item = items.back().get();
        auto item_repetition = item->getRepetition();

        neverRecognizedSuggestion(ruleIterator, item_repetition.to == 0);
        alwaysRecognizedSuggestion(ruleIterator, item_repetition.to != 0 && item->empty());
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
            return;
        }

        auto &last_item = items.back();

        if (last_item->getRepetition() != Repetition::basic()) {
            throwUnableToApply("item already has repetition");
            return;
        }

        last_item->setRepetition(new_repetition);
    }

    auto Container::RuleParser::makeSpecial() -> void
    {
        if (!items.empty()) {
            throwUnableToApply("special must be applied before anything else");
            return;
        }

        container.flags.isSpecial = true;
    }

    auto Container::RuleParser::checkId() const -> void
    {
        if (getId() == std::to_underlying(ReservedTokenType::BAD_TOKEN) ||
            getId() == std::to_underlying(ReservedTokenType::EOI)) {
            throw UnrecoverableError{
                "reserved token type (0 and 1 are reserved for EOI and BAD TOKEN)"};
        }
    }

    auto Container::RuleParser::reverseLastItem() -> void
    {
        if (items.empty()) {
            throwUnableToApply("no items to reverse");
            return;
        }

        auto &last_item = items.back();

        if (last_item->isReversed()) {
            throwUnableToApply("item already has reverse modifier");
            return;
        }

        last_item->reverse();
    }

    auto Container::RuleParser::postCreationCheck() -> void
    {
        const auto postfix_elem = std::ranges::find_if(
            std::as_const(items), [](const auto &elem) { return elem->hasPostfix(); });

        const auto are_postfixes_correct = std::all_of(
            postfix_elem, items.cend(), [](const auto &elem) { return elem->hasPostfix(); });

        if (!are_postfixes_correct) {
            throwUnableToApply("item without postfix modifier exists after items with it");
            return;
        }

        if (constructedLhs.has_value() && !rhsItemConstructed) {
            throwUnableToApply("no rhs items to apply operation");
            return;
        }

        if (constructedLhs.has_value() && rhsItemConstructed) {
            tryToFinishLogicalOperation();
            return;
        }

        finishPreviousItemInitialization();
        BasicItem::neverRecognizedSuggestion(ruleIterator, items.empty() && !isReversed());
        BasicItem::alwaysRecognizedSuggestion(ruleIterator, items.empty() && isReversed());
    }

    auto Container::RuleParser::findContainerEnd(string_view repr) -> size_t
    {
        return *repr.openCloseFind('(', ')').or_else([this]() -> Optional<size_t> {
            ruleIterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, "unterminated dot item");
            throw UnrecoverableError{"unrecoverable error in ContainerType"};
        });
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
            return;
        }

        if (items.empty()) {
            throwUnableToApply("there are not any items to apply prefix/postfix");
            return;
        }

        auto &last_item = items.back();

        if (last_item->hasPrefix()) {
            throwUnableToApply("item already has prefix modifier");
            return;
        }

        if (last_item->hasPostfix()) {
            throwUnableToApply("item already has postfix modifier");
            return;
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
        auto message = "undefined action"_sv;
        auto suggestion =
            "use `!` to declare special symbol, `\"` for string, `[` for unions, `(` for "
            "containers "_sv;

        ruleIterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto BasicItem::SpecialItems::checkForSpecial(const ForkedGenerator &text_iterator) const
        -> bool
    {
        return std::ranges::any_of(special_items, [&text_iterator](const auto &special_item) {
            auto scan_result = special_item.scan(text_iterator);
            return scan_result != 0;
        });
    }

    auto BasicItem::SpecialItems::specialScan(TextIterator &text_iterator, Token &token) const
        -> bool
    {
        return std::ranges::any_of(
            special_items, [&text_iterator, &token](const auto &special_item) {
                return special_item.beginScan(text_iterator, token, ScanningType::SPECIAL);
            });
    }
}// namespace ccl::lex::dot_item
