#include <cerberus/format/format.hpp>
#include <cerberus/lex/dot_item/items_counter.hpp>

namespace cerb::lex::dot_item
{
    auto ItemsCounter::add(item::UnionType /* unused */) -> void
    {
        checkForUnexpectedTerminals<u8"union">();
        checkNoStringOrChars<u8"union">();
        ++unions;
    }

    auto ItemsCounter::add(item::DotItemType /* unused */) -> void
    {
        checkForUnexpectedTerminals<u8"dot item">();
        checkNoStringOrChars<u8"dot item">();
        ++dot_items;
    }

    auto ItemsCounter::add(item::SequenceType /* unused */) -> void
    {
        checkForUnexpectedTerminals<u8"sequence">();
        checkAbilityToCreateSequence();
        ++sequences;
    }

    auto ItemsCounter::add(item::StringType /* unused */) -> void
    {
        checkForUnexpectedTerminals<u8"string">();
        checkThereIsOneSequence<u8"string like item">();
        --sequences;
        ++strings;
    }

    auto ItemsCounter::add(item::CharacterType /* unused */) -> void
    {
        checkForUnexpectedTerminals<u8"character">();
        checkThereIsOneSequence<u8"string like item">();
        --sequences;
        ++characters;
    }

    auto ItemsCounter::add(item::TerminalType /* unused */) -> void
    {
        checkAbilityToCreateTerminal();
        ++terminals;
    }

    auto ItemsCounter::add(item::CommentType /* unused */) -> void
    {
        checkForUnexpectedTerminals<u8"comment">();
        checkThereIsOneSequence<u8"comment">();
        --sequences;
    }

    auto ItemsCounter::checkAbilityToCreateSequence() -> void
    {
        if (land(hasStrOrChar(), hasSequences())) {
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

    template<ConstString ItemName>
    auto ItemsCounter::checkThereIsOneSequence() -> void
    {
        if (sequences == 0) {
            throwItemCreationError<ItemName, u8"no sequences found", u8"create sequence">();
        }

        if (sequences > 1) {
            throwItemCreationError<
                ItemName, u8"too many sequences found", u8"delete sequences or join them">();
        }

        if (lor(hasUnions(), hasDotItems())) {
            throwItemCreationError<
                ItemName, u8"because non sequence item exists in the rule",
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
