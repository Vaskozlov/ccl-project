#include <ccl/lexer/rule/binary_expression/binary_operation_and.hpp>
#include <ccl/lexer/rule/binary_expression/binary_operation_or.hpp>
#include <ccl/lexer/rule/container.hpp>
#include <ccl/lexer/rule/rule_reference.hpp>
#include <ccl/lexer/rule/sequence.hpp>
#include <ccl/lexer/rule/union.hpp>
#include <utility>

namespace ccl::lexer::rule
{
    Container::RuleParser::RuleParser(Container &target_container, TextIterator &text_iterator)
      : container{target_container}
      , ruleIterator{text_iterator}
      , lexicalAnalyzer{target_container.lexicalAnalyzer}
    {
        ruleIterator.moveToCleanChar();

        while (hasMovedToTheNextChar()) {
            recognizeAction();
            ruleIterator.moveToCleanChar();
        }

        postCreationCheck();
    }

    auto Container::RuleParser::hasMovedToTheNextChar() const -> bool
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

        case U'\'':
            emplaceItem(constructNewRuleReference());
            break;

        case U'(':
            emplaceItem(constructNewContainer());
            break;

        case U'*':
            setRepetition(Repetition{0, Repetition::max()});
            break;

        case U'+':
            setRepetition(Repetition{1, Repetition::max()});
            break;

        case U'?':
            setRepetition(Repetition{0, 1});
            break;

        case U'{':
            setRepetition(Repetition{ruleIterator});
            break;

        case U'^':
            reverseLastItem();
            break;

        case U'e':
            makeLastItemExtractable();
            break;

        case U'h':
            hideFromParser();
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

    auto Container::RuleParser::constructBinaryExpression() -> RuleBlock
    {
        auto rhs = std::move(items.back());
        items.pop_back();

        if (binaryOperator == BinaryOperator::AND) {
            return RuleBlock{BinaryOperationAnd(std::move(constructedLhs.value()), std::move(rhs))};
        }

        if (binaryOperator == BinaryOperator::OR) {
            return RuleBlock{BinaryOperationOr(std::move(constructedLhs.value()), std::move(rhs))};
        }

        isl::unreachable();
    }

    auto Container::RuleParser::constructNewSequence() -> RuleBlock
    {
        tryToFinishBinaryExpression();

        return RuleBlock{Sequence{Sequence::SequenceFlags{}, "\"", ruleIterator}};
    }

    auto Container::RuleParser::constructNewRuleReference() -> RuleBlock
    {
        tryToFinishBinaryExpression();

        return RuleBlock{RuleReference{lexicalAnalyzer, "\'", "\'", ruleIterator}};
    }

    auto Container::RuleParser::constructNewUnion() -> RuleBlock
    {
        tryToFinishBinaryExpression();

        return RuleBlock{Union{ruleIterator}};
    }

    // NOLINTNEXTLINE (recursive function)
    auto Container::RuleParser::constructNewContainer() -> RuleBlock
    {
        tryToFinishBinaryExpression();

        auto text = ruleIterator.getRemainingWithCurrent();
        const auto *saved_end = ruleIterator.getEnd();
        auto bracket_index = findContainerEnd(text);

        ruleIterator.setEnd(text.begin() + bracket_index);

        auto new_container =
            Container{lexicalAnalyzer, ruleIterator, false, container.isAnyPlaceItem()};
        ruleIterator.setEnd(saved_end);

        return RuleBlock{std::move(new_container)};
    }

    auto Container::RuleParser::emplaceItem(std::derived_from<RuleBlockInterface> auto item) -> void
    {
        if (!item.canBeOptimized()) {
            completePreviousItemInitialization();
            items.emplace_back(std::move(item));
        }
    }

    auto Container::RuleParser::emplaceItem(RuleBlock item) -> void
    {
        if (!item->canBeOptimized()) {
            completePreviousItemInitialization();
            items.emplace_back(std::move(item));
        }
    }

    auto Container::RuleParser::completePreviousItemInitialization() -> void
    {
        if (items.empty()) {
            return;
        }

        const RuleBlock &item = items.back();
        auto item_repetition = item->getRepetition();

        neverRecognizedSuggestion(
            ruleIterator, item_repetition.to == 0 || (item->empty() && !item->isReversed()));

        alwaysRecognizedSuggestion(
            ruleIterator, item_repetition.to != 0 && item->empty() && item->isReversed());
    }

    auto Container::RuleParser::hideFromParser() const -> void
    {
        auto &last_item = items.back();
        last_item->hideFromParser();
    }

    auto Container::RuleParser::makeLastItemExtractable() -> void
    {
        checkAbilityToCreatePrefixOrPostfix();

        const auto &last_item = items.back();
        last_item->makeExtractable();
    }

    auto Container::RuleParser::setRepetition(const Repetition new_repetition) const -> void
    {
        if (items.empty()) {
            throwUnableToApply("no items found to set repetition");
        }

        const auto &last_item = items.back();

        if (last_item->getRepetition() != Repetition{1, 1}) {
            throwUnableToApply("item already has repetition");
        }

        last_item->setRepetition(new_repetition);
    }

    auto Container::RuleParser::makeSpecial() const -> void
    {
        if (!items.empty()) {
            throwUnableToApply("special must be applied before anything else");
        }

        container.flags.isAnyPlace = true;
    }

    auto Container::RuleParser::reverseLastItem() const -> void
    {
        if (items.empty()) {
            throwUnableToApply("no items to reverse");
        }

        const auto &last_item = items.back();

        if (last_item->isReversed()) {
            throwUnableToApply("item already has reverse modifier");
        }

        last_item->reverse();
    }

    auto Container::RuleParser::postCreationCheck() -> void
    {
        if (constructedLhs.has_value() && !rhsItemConstructed) {
            throwUnableToApply("no rhs items to apply operation");
        }

        if (constructedLhs.has_value() && rhsItemConstructed) {
            tryToFinishBinaryExpression();
            return;
        }

        completePreviousItemInitialization();
        neverRecognizedSuggestion(ruleIterator, items.empty() && !isReversed());
        alwaysRecognizedSuggestion(ruleIterator, items.empty() && isReversed());
    }

    auto Container::RuleParser::findContainerEnd(const isl::string_view repr) const -> std::size_t
    {
        const auto closing_bracket_index = repr.findRangeEnd('(', ')');

        if (closing_bracket_index != isl::string_view::npos) {
            return closing_bracket_index;
        }

        ruleIterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, "unterminated dot item");
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto Container::RuleParser::checkThereIsLhsItem() const -> void
    {
        if (items.empty()) [[unlikely]] {
            throwUnableToApply("no left hand side items to apply operation");
        }
    }

    auto Container::RuleParser::checkAbilityToCreatePrefixOrPostfix() const -> void
    {
        if (!container.flags.isMain) {
            throwUnableToApply(
                "you are not allowed to create prefixes or postfixes inside other containers");
        }

        if (items.empty()) {
            throwUnableToApply("there are not any items to apply prefix/postfix");
        }

        const auto &last_item = items.back();

        if (last_item->isExtractable()) {
            throwUnableToApply("item is already extractable");
        }
    }

    auto Container::RuleParser::throwUnableToApply(
        const isl::string_view reason,
        const isl::string_view suggestion) const -> void
    {
        ruleIterator.throwCriticalError(
            AnalysisStage::LEXICAL_ANALYSIS,
            fmt::format("unable to apply: {}", reason),
            suggestion);
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto Container::RuleParser::throwUndefinedAction() const -> void
    {
        using namespace isl::string_view_literals;

        constexpr static auto message = "undefined action"_sv;
        constexpr static auto suggestion =
            "use `!` to declare special symbol, `\"` for string, `[` for unions, `(` for "
            "containers "_sv;

        ruleIterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
        throw UnrecoverableError{"unrecoverable error in ContainerType"};
    }

    auto RuleBlockInterface::AnyPlaceItems::checkForSpecial(
        const ForkedGenerator &text_iterator) const -> bool
    {
        return std::ranges::any_of(items, [&text_iterator](const auto &special_item) {
            return special_item->scan(text_iterator).isSuccess();
        });
    }

    auto RuleBlockInterface::AnyPlaceItems::isSuccessfulScan(
        TextIterator &text_iterator,
        Token &token) const -> bool
    {
        return std::ranges::any_of(items, [&text_iterator, &token](const auto &special_item) {
            return special_item->beginScan(text_iterator, token, ScanType::SPECIAL);
        });
    }
}// namespace ccl::lexer::rule
