#include <ccl/lex/dot_item/container.hpp>
#include <ccl/lex/dot_item/sequence.hpp>
#include <ccl/lex/dot_item/union.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    Container::RuleParser::RuleParser(Container &container_, TextIterator &rule_iterator_)
      : container(container_), rule_iterator(rule_iterator_)
    {
        checkId();

        rule_iterator.moveToCleanChar();

        while (hasMovedToTheNextChar()) {
            recognizeAction();
            rule_iterator.moveToCleanChar();
        }

        postCreationCheck();
    }

    auto Container::RuleParser::hasMovedToTheNextChar() -> bool
    {
        return not isEoF(rule_iterator.next());
    }

    // NOLINTNEXTLINE recursive function
    auto Container::RuleParser::recognizeAction() -> void
    {
        switch (rule_iterator.getCurrentChar()) {
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
            addRecurrence(Recurrence::star());
            break;

        case U'+':
            addRecurrence(Recurrence::plus());
            break;

        case U'?':
            addRecurrence(Recurrence::question());
            break;

        case U'{':
            addRecurrence(Recurrence{ rule_iterator });
            break;

        case U'^':
            reverseLastItem();
            break;

        case U'p':
            checkAbilityToCreatePrefixPostfix();
            addPrefixPostfix();
            break;

        case U'&':
            prepareForLogicalOperation(LogicalUnitType::AND);
            break;

        case U'|':
            prepareForLogicalOperation(LogicalUnitType::OR);
            break;

        default:
            throwUndefinedAction();
            break;
        }
    }

    auto Container::RuleParser::prepareForLogicalOperation(LogicalUnitType type) -> void
    {
        checkThereIsLhsItem();
        logical_unit_type = type;

        reserved_lhs = std::move(items.back());
        items.pop_back();
    }

    auto Container::RuleParser::tryToFinishLogicalOperation() -> void
    {
        if (reserved_lhs.has_value() && not rhs_item_constructed) {
            rhs_item_constructed = true;
        } else if (rhs_item_constructed && reserved_lhs.has_value()) {
            emplaceItem(constructLogicalUnit());
            logical_unit_type = LogicalUnitType::NONE;
        }
    }

    auto Container::RuleParser::constructLogicalUnit() -> std::unique_ptr<BasicItem>
    {
        auto rhs = std::move(items.back());
        items.pop_back();

        return std::make_unique<LogicalUnit>(
            std::move(reserved_lhs.value()), std::move(rhs), logical_unit_type, special_items);
    }

    auto Container::RuleParser::constructNewSequence() -> std::unique_ptr<BasicItem>
    {
        tryToFinishLogicalOperation();

        return std::make_unique<Sequence>(
            Sequence::SequenceFlags{}, "\"", rule_iterator, special_items);
    }

    auto Container::RuleParser::constructNewUnion() -> std::unique_ptr<BasicItem>
    {
        tryToFinishLogicalOperation();

        return std::make_unique<Union>(rule_iterator, special_items);
    }

    // NOLINTNEXTLINE (recursive function)
    auto Container::RuleParser::constructNewContainer() -> std::unique_ptr<BasicItem>
    {
        tryToFinishLogicalOperation();

        auto text = rule_iterator.getRemainingWithCurrent();
        const auto *saved_end = rule_iterator.getEnd();
        auto bracket_index = findContainerEnd(text);

        rule_iterator.setEnd(text.begin() + bracket_index);

        auto new_container =
            std::make_unique<Container>(rule_iterator, container.id, special_items, false);
        rule_iterator.setEnd(saved_end);

        return new_container;
    }

    auto Container::RuleParser::emplaceItem(std::unique_ptr<BasicItem> &&item) -> void
    {
        if (not item->canBeOptimized()) {
            auto item_recurrence = item->getRecurrence();

            BasicItem::neverRecognizedSuggestion(
                rule_iterator, item_recurrence.from == 0 && not isReversed() && not item->empty());

            BasicItem::alwaysRecognizedSuggestion(rule_iterator, not isReversed() && item->empty());

            items.emplace_back(std::move(item));
        }
    }

    auto Container::RuleParser::addPrefixPostfix() -> void
    {
        auto &last_item = items.back();
        auto is_prefix = items.size() == 1 || items[items.size() - 2]->hasPrefix();

        if (is_prefix) {
            last_item->setPrefix();
        } else {
            last_item->setPostfix();
        }
    }

    auto Container::RuleParser::addRecurrence(Recurrence new_recurrence) -> void
    {
        if (items.empty()) {
            throwUnableToApply("no items found", "set recurrence modifier after item");
            return;
        }

        auto &last_item = items.back();

        if (last_item->getRecurrence() != Recurrence::basic()) {
            throwUnableToApply(
                "item already has recurrence", "do not set recurrence more than once");
            return;
        }

        last_item->setRecurrence(new_recurrence);
    }

    auto Container::RuleParser::makeSpecial() -> void
    {
        if (not items.empty()) {
            throwUnableToApply("special must be applied before anything else");
            return;
        }

        container.is_special = true;
    }

    auto Container::RuleParser::checkId() const -> void
    {
        if (ReservedTokenType::contains(getId())) {
            throw UnrecoverableError{
                "reserved token type (0 and 1 are reserved for EOI and BAD TOKEN)"
            };
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
        auto postfix_elem =
            std::ranges::find_if(items, [](const auto &elem) { return elem->hasPostfix(); });

        auto are_postfixes_correct = std::all_of(
            postfix_elem, items.end(), [](const auto &elem) { return elem->hasPostfix(); });

        if (not are_postfixes_correct) {
            auto suggestion = fmt::format(
                "add postfix modifier to the last item\n{}p", rule_iterator.getWorkingLine());

            throwUnableToApply(
                "item without postfix modifier exists after items with it", suggestion);
            return;
        }

        if (reserved_lhs.has_value() && not rhs_item_constructed) {
            throwUnableToApply("no rhs items to apply operation");
        }

        if (reserved_lhs.has_value() && rhs_item_constructed) {
            tryToFinishLogicalOperation();
        }

        BasicItem::neverRecognizedSuggestion(rule_iterator, items.empty() && not isReversed());
        BasicItem::alwaysRecognizedSuggestion(rule_iterator, items.empty() && isReversed());
    }

    auto Container::RuleParser::findContainerEnd(string_view repr) -> size_t
    {
        auto bracket_index = repr.openCloseFind('(', ')');

        if (not bracket_index.has_value()) {
            rule_iterator.throwPanicError("unterminated dot item");
            throw UnrecoverableError{ "unrecoverable error in ContainerType" };
        }

        return *bracket_index;
    }

    auto Container::RuleParser::checkThereIsLhsItem() -> void
    {
        if (items.empty()) {
            throwUnableToApply("no left hand side items to apply operation");
        }
    }

    auto Container::RuleParser::checkAbilityToCreatePrefixPostfix() -> void
    {
        if (not container.main_item) {
            throwUnableToApply(
                "you are not allowed to create prefixes or postfixes inside other dot items",
                "create them only in main item");
            return;
        }

        if (items.empty()) {
            throwUnableToApply("there are not any items to apply prefix/postfix");
            return;
        }

        auto &last_item = items.back();

        if (last_item->hasPrefix()) {
            throwUnableToApply("item already has prefix modifier", "do not add it more than once");
            return;
        }

        if (last_item->hasPostfix()) {
            throwUnableToApply("item already has postfix modifier", "do not add it more than once");
            return;
        }
    }

    auto Container::RuleParser::throwUnableToApply(string_view reason, string_view suggestion)
        -> void
    {
        using namespace fmt::literals;

        auto message = fmt::format("unable to apply: {}", reason);

        rule_iterator.throwCriticalError(message, suggestion);
        throw UnrecoverableError{ "unrecoverable error in ContainerType" };
    }

    auto Container::RuleParser::throwUndefinedAction() -> void
    {
        auto message = "undefined action"_sv;
        auto suggestion =
            "Use `\"` for string, `'` for special symbol, `[` for unions, `(` for dot "
            "items"_sv;

        rule_iterator.throwPanicError(message, suggestion);
        throw UnrecoverableError{ "unrecoverable error in ContainerType" };
    }
}// namespace ccl::lex::dot_item
