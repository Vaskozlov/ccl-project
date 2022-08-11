#ifndef CERBERUS_PROJECT_ITEMS_COUNTER_HPP
#define CERBERUS_PROJECT_ITEMS_COUNTER_HPP

#include <cerberus/format/format.hpp>
#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    CERBLIB_EXCEPTION(DotItemException, text::TextIteratorException);

    namespace item
    {
        struct UnionType : std::true_type
        {
        };

        struct SequenceType : std::true_type
        {
        };

        struct DotItemType : std::true_type
        {
        };

        struct StringType : std::true_type
        {
        };

        struct CharacterType : std::true_type
        {
        };

        struct TerminalType : std::true_type
        {
        };

        constexpr auto Union = UnionType{};
        constexpr auto Sequence = SequenceType{};
        constexpr auto DotItem = DotItemType{};
        constexpr auto String = StringType{};
        constexpr auto Character = CharacterType{};
        constexpr auto Terminal = TerminalType{};
    }// namespace item

    struct ItemsCounter
    {
        [[nodiscard]] auto hasStrings() const -> bool
        {
            return strings != 0;
        }

        [[nodiscard]] auto hasCharacters() const -> bool
        {
            return characters != 0;
        }

        [[nodiscard]] auto hasUnions() const -> bool
        {
            return unions != 0;
        }

        [[nodiscard]] auto hasSequences() const -> bool
        {
            return sequences != 0;
        }

        [[nodiscard]] auto hasDotItems() const -> bool
        {
            return dot_items != 0;
        }

        [[nodiscard]] auto hasTerminals() const -> bool
        {
            return terminals != 0;
        }

        [[nodiscard]] auto hasStrOrChar() const -> bool
        {
            return lor(hasStrings(), hasCharacters());
        }

        auto operator+=(item::UnionType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals(u8"union");
            checkNoStringOrChars(u8"union");
            ++unions;
            return *this;
        }

        auto operator+=(item::DotItemType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals(u8"dot item");
            checkNoStringOrChars(u8"dot item");
            ++dot_items;
            return *this;
        }

        auto operator+=(item::SequenceType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals(u8"sequence");
            checkAbilityToCreateSequence();
            ++sequences;
            return *this;
        }

        auto operator+=(item::StringType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals(u8"string");
            checkAbilityToCreateStringOrCharacter();
            --sequences;
            ++strings;
            return *this;
        }

        auto operator+=(item::CharacterType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals(u8"character");
            checkAbilityToCreateStringOrCharacter();
            --sequences;
            ++characters;
            return *this;
        }

        auto operator+=(item::TerminalType /* unused */) -> ItemsCounter &
        {
            checkAbilityToCreateTerminal();
            ++terminals;
            return *this;
        }

        explicit ItemsCounter(text::TextIterator &text_iterator_) : text_iterator(text_iterator_)
        {}

        size_t strings{};
        size_t characters{};
        size_t unions{};
        size_t sequences{};
        size_t dot_items{};
        size_t terminals{};

    private:
        auto checkAbilityToCreateSequence() -> void
        {
            if (hasStrOrChar() && hasSequences()) {
                throwItemCreationError(
                    u8"sequence", u8"attempt to create sequence in character/string item",
                    u8"do not declare sequences in string/character item");
            }
        }

        auto checkAbilityToCreateTerminal() -> void
        {
            if ((characters + strings + dot_items + sequences + unions + terminals) != terminals) {
                terminals = std::max<size_t>(terminals, 1);
                checkForUnexpectedTerminals(u8"terminal");// just for the same error message
            }
        }

        auto checkAbilityToCreateStringOrCharacter() -> void
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

        auto checkForUnexpectedTerminals(std::u8string_view item_name) -> void
        {
            if (hasTerminals()) {
                throwItemCreationError(item_name, u8"terminals cannot coexist with other items");
            }
        }

        auto checkNoStringOrChars(std::u8string_view item_name) -> void
        {
            if (hasStrOrChar()) {
                throwItemCreationError(
                    item_name, u8"string or characters has been already declared in item",
                    u8"delete strings/characters or do not declare other items");
            }
        }

        auto throwItemCreationError(
            std::u8string_view item_name,
            std::u8string_view message,
            std::u8string_view suggestion = {}) -> void
        {
            auto error_message = fmt::format<u8"unable to create {}: {}">(item_name, message);
            auto error_suggestion = fmt::format<u8"{}">(suggestion);

            text_iterator.template throwException<DotItemException>(
                error_message, error_suggestion);
            throw UnrecoverableError{ "unrecoverable error in DotItemType" };
        }

        text::TextIterator &text_iterator;
    };
}// namespace cerb::lex::dot_item

#endif /*  CERBERUS_PROJECT_ITEMS_COUNTER_HPP */
