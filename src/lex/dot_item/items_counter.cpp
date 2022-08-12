#include <cerberus/lex/dot_item/items_counter.hpp>

namespace cerb::lex::dot_item
{
    auto ItemsCounter::operator+=(item::UnionType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals(u8"union");
        checkNoStringOrChars(u8"union");
        ++unions;
        return *this;
    }

    auto ItemsCounter::operator+=(item::DotItemType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals(u8"dot item");
        checkNoStringOrChars(u8"dot item");
        ++dot_items;
        return *this;
    }

    auto ItemsCounter::operator+=(item::SequenceType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals(u8"sequence");
        checkAbilityToCreateSequence();
        ++sequences;
        return *this;
    }

    auto ItemsCounter::operator+=(item::StringType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals(u8"string");
        checkAbilityToCreateStringOrCharacter();
        --sequences;
        ++strings;
        return *this;
    }

    auto ItemsCounter::operator+=(item::CharacterType /* unused */) -> ItemsCounter &
    {
        checkForUnexpectedTerminals(u8"character");
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
            throwItemCreationError(
                u8"sequence", u8"attempt to create sequence in character/string item",
                u8"do not declare sequences in string/character item");
        }
    }

    auto ItemsCounter::checkAbilityToCreateTerminal() -> void
    {
        if ((characters + strings + dot_items + sequences + unions + terminals) != terminals) {
            terminals = std::max<size_t>(terminals, 1);
            checkForUnexpectedTerminals(u8"terminal");// just for the same error message
        }
    }

    auto ItemsCounter::checkAbilityToCreateStringOrCharacter() -> void
    {
        if (sequences != 1) {
            throwItemCreationError(
                u8"string/character",
                u8"attempt to create string/character item without sequence",
                u8"create sequence before creating string/character item");
        }

        if (hasUnions() || hasDotItems()) {
            throwItemCreationError(
                u8"string/character",
                u8"attempt to create string/character item after union/dot item",
                u8"do not declare strings/characters in item with anything else");
        }
    }

    auto ItemsCounter::checkForUnexpectedTerminals(std::u8string_view item_name) -> void
    {
        if (hasTerminals()) {
            throwItemCreationError(item_name, u8"terminals cannot coexist with other items");
        }
    }

    auto ItemsCounter::checkNoStringOrChars(std::u8string_view item_name) -> void
    {
        if (hasStrOrChar()) {
            throwItemCreationError(
                item_name, u8"string or characters has been already declared in item",
                u8"delete strings/characters or do not declare other items");
        }
    }

    auto ItemsCounter::throwItemCreationError(
        std::u8string_view item_name,
        std::u8string_view message,
        std::u8string_view suggestion) -> void
    {
        auto error_message = fmt::format<u8"unable to create {}: {}">(item_name, message);
        auto error_suggestion = fmt::format<u8"{}">(suggestion);

        text_iterator.template throwException<DotItemException>(error_message, error_suggestion);
        throw UnrecoverableError{ "unrecoverable error in DotItemType" };
    }
}// namespace cerb::lex::dot_item
