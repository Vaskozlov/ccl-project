#include <cerberus/format/format.hpp>
#include <cerberus/lex/dot_item/items_counter.hpp>

namespace cerb::lex::dot_item
{
    auto ItemsCounter::operator+=(item::UnionType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals<u8"union">();
        checkNoStringOrChars<u8"union">();
        ++unions;
        return *this;
    }

    auto ItemsCounter::operator+=(item::DotItemType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals<u8"dot item">();
        checkNoStringOrChars<u8"dot item">();
        ++dot_items;
        return *this;
    }

    auto ItemsCounter::operator+=(item::SequenceType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals<u8"sequence">();
        checkAbilityToCreateSequence();
        ++sequences;
        return *this;
    }

    auto ItemsCounter::operator+=(item::StringType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals<u8"string">();
        checkAbilityToCreateStringOrCharacter();
        --sequences;
        ++strings;
        return *this;
    }

    auto ItemsCounter::operator+=(item::CharacterType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals<u8"character">();
        checkAbilityToCreateStringOrCharacter();
        --sequences;
        ++characters;
        return *this;
    }

    auto ItemsCounter::operator+=(item::TerminalType /* unused */) -> ItemsCounter &
    {
        checkAbilityToCreateTerminal();
        ++terminals;
        return *this;
    }

    auto ItemsCounter::checkAbilityToCreateSequence() -> void
    {
        if (hasStrOrChar() && hasSequences()) {
            throwItemCreationError<
                u8"sequence", u8"attempt to create sequence in string like item",
                u8"do not declare sequences in string like item">();
        }
    }

    auto ItemsCounter::checkAbilityToCreateTerminal() -> void
    {
        if ((characters + strings + dot_items + sequences + unions + terminals) != terminals) {
            terminals = std::max<size_t>(terminals, 1);
            checkForUnexpectedTerminals<u8"terminal">();// just for the same error message
        }
    }

    auto ItemsCounter::checkAbilityToCreateStringOrCharacter() -> void
    {
        if (sequences == 0) {
            throwItemCreationError<
                u8"string like item", u8"no sequences found", u8"create sequence">();
        }

        if (sequences > 1) {
            throwItemCreationError<
                u8"string like item", u8"too many sequences found",
                u8"delete sequences or join them">();
        }

        if (hasUnions() || hasDotItems()) {
            throwItemCreationError<
                u8"string like item", u8"because non sequence item exists in the rule",
                u8"delete Union/DotItem">();
        }
    }

    template<ConstString ItemName>
    auto ItemsCounter::checkForUnexpectedTerminals() -> void
    {
        if (hasTerminals()) {
            throwItemCreationError<ItemName, u8"terminals cannot coexist with other items", u8"">();
        }
    }

    template<ConstString ItemName>
    auto ItemsCounter::checkNoStringOrChars() -> void
    {
        if (hasStrOrChar()) {
            throwItemCreationError<
                ItemName, u8"string or characters has been already declared in expression",
                u8"delete string like items or do not use other items">();
        }
    }

    template<ConstString ItemName, ConstString Message, ConstString Suggestion>
    auto ItemsCounter::throwItemCreationError() -> void
    {
        constexpr static auto error_message =
            fmt::staticFormat<u8"unable to create {}: {}", ItemName, Message>();

        text_iterator.throwException<DotItemException>(error_message, Suggestion);
        throw UnrecoverableError{ "unrecoverable error in DotItemType" };
    }
}// namespace cerb::lex::dot_item
