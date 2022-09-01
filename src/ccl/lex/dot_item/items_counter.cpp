#include <ccl/lex/dot_item/items_counter.hpp>

namespace ccl::lex::dot_item
{
    auto ItemsCounter::add(item::UnionType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<"union">();
        checkNoStringOrChars<"union">();
        ++unions;
    }

    auto ItemsCounter::add(item::ContainerType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<"dot item">();
        checkNoStringOrChars<"dot item">();
        ++containers;
    }

    auto ItemsCounter::add(item::SequenceType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<"sequence">();
        checkAbilityToCreateSequence();
        ++sequences;
    }

    auto ItemsCounter::add(item::StringType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<"string">();
        checkThereIsOneSequence<"string like item">();
        --sequences;
        ++strings;
    }

    auto ItemsCounter::add(item::CharacterType /* unused */) -> void
    {
        checkForUnexpectedSpecialSymbols<"character">();
        checkThereIsOneSequence<"string like item">();
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
        checkForUnexpectedSpecialSymbols<"comment">();
        checkThereIsOneSequence<"comment">();
        --sequences;
        ++comments;
    }

    auto ItemsCounter::checkAbilityToCreateSequence() -> void
    {
        if (land(hasStrOrChar(), hasSequences())) {
            throwItemCreationError(
                "sequence",
                "attempt to create sequence in string like item",
                "do not declare sequences in string like item");
        }
    }

    auto ItemsCounter::checkAbilityToCreateTerminal() -> void
    {
        if (sumAll() != special_tokens) {
            special_tokens = std::max<size_t>(special_tokens, 1);
            checkForUnexpectedSpecialSymbols<"special symbol">();// just for the same error message
        }
    }

    template<ConstString ItemName>
    auto ItemsCounter::checkThereIsOneSequence() -> void
    {
        if (sequences == 0) {
            throwItemCreationError(ItemName, "no sequences found", "create sequence");
        }

        if (sequences > 1) {
            throwItemCreationError(
                ItemName, "too many sequences found", "delete sequences or join them");
        }

        if (lor(hasUnions(), hasContainers())) {
            throwItemCreationError(
                ItemName, "because non sequence item exists in the rule", "delete Union/Container");
        }
    }

    template<ConstString ItemName>
    auto ItemsCounter::checkForUnexpectedSpecialSymbols() -> void
    {
        if (hasTerminals()) {
            throwItemCreationError(ItemName, "special symbols cannot coexist with other items", "");
        }
    }

    template<ConstString ItemName>
    auto ItemsCounter::checkNoStringOrChars() -> void
    {
        if (hasStrOrChar()) {
            throwItemCreationError(
                ItemName,
                "string or characters has been already declared in expression",
                "delete string like items or do not use other items");
        }
    }

    auto ItemsCounter::throwItemCreationError(
        string_view item_name,
        string_view message,
        string_view suggestion) -> void
    {
        auto error_message = fmt::format("unable to create {}: {}", item_name, message);

        text_iterator.throwPanicError(error_message, suggestion);
        throw UnrecoverableError{ "unrecoverable error in ContainerType" };
    }
}// namespace ccl::lex::dot_item
