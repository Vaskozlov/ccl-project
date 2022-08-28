#include <ccl/format/format.hpp>
#include <ccl/lex/dot_item/items_counter.hpp>

namespace ccl::lex::dot_item
{
    auto ItemsCounter::add(item::UnionType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<u8"union">();
        checkNoStringOrChars<u8"union">();
        ++unions;
    }

    auto ItemsCounter::add(item::ContainerType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<u8"dot item">();
        checkNoStringOrChars<u8"dot item">();
        ++containers;
    }

    auto ItemsCounter::add(item::SequenceType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<u8"sequence">();
        checkAbilityToCreateSequence();
        ++sequences;
    }

    auto ItemsCounter::add(item::StringType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<u8"string">();
        checkThereIsOneSequence<u8"string like item">();
        --sequences;
        ++strings;
    }

    auto ItemsCounter::add(item::CharacterType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<u8"character">();
        checkThereIsOneSequence<u8"string like item">();
        --sequences;
        ++characters;
    }

    auto ItemsCounter::add(item::TerminalType /* unused */) -> void
    {
        checkAbilityToCreateTerminal();
        ++special_tokens;
    }

    auto ItemsCounter::add(item::CommentType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<u8"comment">();
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
        if ((characters + strings + containers + sequences + unions + special_tokens) !=
            special_tokens) {
            special_tokens = std::max<size_t>(special_tokens, 1);
            checkForUnexpectedSpecialSymbols<u8"special symbol">();// just for the same error message
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

        if (lor(hasUnions(), hasContainers())) {
            throwItemCreationError<
                ItemName, u8"because non sequence item exists in the rule",
                u8"delete Union/Container">();
        }
    }

    template<ConstString ItemName>
    auto ItemsCounter::checkForUnexpectedSpecialSymbols() -> void
    {
        if (hasTerminals()) {
            throwItemCreationError<ItemName, u8"special symbols cannot coexist with other items", u8"">();
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

        text_iterator.throwException<ContainerException>(error_message, Suggestion);
        throw UnrecoverableError{ "unrecoverable error in ContainerType" };
    }
}// namespace ccl::lex::dot_item
