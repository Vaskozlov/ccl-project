#ifndef CERBERUS_PROJECT_ITEMS_COUNTER_HPP
#define CERBERUS_PROJECT_ITEMS_COUNTER_HPP

#include <cerberus/format/format.hpp>
#include <cerberus/lex/typedefs.hpp>
#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(DotItemException, text::TextIteratorException<CharT>);

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

    template<CharacterLiteral CharT>
    struct ItemsCounter
    {
        CERBLIB_DECL auto hasStrings() const -> bool
        {
            return strings != 0;
        }

        CERBLIB_DECL auto hasCharacters() const -> bool
        {
            return characters != 0;
        }

        CERBLIB_DECL auto hasUnions() const -> bool
        {
            return unions != 0;
        }

        CERBLIB_DECL auto hasSequences() const -> bool
        {
            return sequences != 0;
        }

        CERBLIB_DECL auto hasDotItems() const -> bool
        {
            return dot_items != 0;
        }

        CERBLIB_DECL auto hasTerminals() const -> bool
        {
            return terminals != 0;
        }

        CERBLIB_DECL auto hasStrOrChar() const -> bool
        {
            return lor(hasStrings(), hasCharacters());
        }

        constexpr auto operator+=(item::UnionType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals("union");
            checkNoStringOrChars("union");
            ++unions;
            return *this;
        }

        constexpr auto operator+=(item::DotItemType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals("dot item");
            checkNoStringOrChars("dot item");
            ++dot_items;
            return *this;
        }

        constexpr auto operator+=(item::SequenceType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals("sequence");
            checkAbilityToCreateSequence();
            ++sequences;
            return *this;
        }

        constexpr auto operator+=(item::StringType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals("string");
            checkAbilityToCreateStringOrCharacter();
            --sequences;
            ++strings;
            return *this;
        }

        constexpr auto operator+=(item::CharacterType /* unused */) -> ItemsCounter &
        {
            checkForUnexpectedTerminals("character");
            checkAbilityToCreateStringOrCharacter();
            --sequences;
            ++characters;
            return *this;
        }

        constexpr auto operator+=(item::TerminalType /* unused */) -> ItemsCounter &
        {
            checkAbilityToCreateTerminal();
            ++terminals;
            return *this;
        }

        ItemsCounter() = default;

        constexpr explicit ItemsCounter(text::TextIterator<CharT> &text_iterator_)
          : text_iterator(text_iterator_)
        {}

        size_t strings{};
        size_t characters{};
        size_t unions{};
        size_t sequences{};
        size_t dot_items{};
        size_t terminals{};

    private:
        constexpr auto checkAbilityToCreateSequence() -> void
        {
            if (hasStrOrChar() && hasSequences()) {
                throwItemCreationError(
                    "sequence", "attempt to create sequence in character/string item",
                    "do not declare sequences in string/character item");
            }
        }

        constexpr auto checkAbilityToCreateTerminal() -> void
        {
            if ((characters + strings + dot_items + sequences + unions + terminals) != terminals) {
                terminals = std::max<size_t>(terminals, 1);
                checkForUnexpectedTerminals("terminal");// just for the same error message
            }
        }

        constexpr auto checkAbilityToCreateStringOrCharacter() -> void
        {
            if (sequences != 1) {
                throwItemCreationError(
                    "string/character",
                    "attempt to create string/character item without sequence",
                    "create sequence before creating string/character item");
            }

            if (hasUnions() || hasDotItems()) {
                throwItemCreationError(
                    "string/character",
                    "attempt to create string/character item after union/dot item",
                    "do not declare strings/characters in item with anything else");
            }
        }

        constexpr auto checkForUnexpectedTerminals(string_view item_name) -> void
        {
            if (hasTerminals()) {
                throwItemCreationError(item_name, "terminals cannot coexist with other items");
            }
        }

        constexpr auto checkNoStringOrChars(string_view item_name) -> void
        {
            if (hasStrOrChar()) {
                throwItemCreationError(
                    item_name, "string or characters has been already declared in item",
                    "delete strings/characters or do not declare other items");
            }
        }

        constexpr auto throwItemCreationError(
            string_view item_name,
            string_view message,
            string_view suggestion = {}) -> void
        {
            auto error_message = fmt::format<CharT, "unable to create {}: {}">(item_name, message);
            auto error_suggestion = fmt::format<CharT, "{}">(suggestion);

            text_iterator.throwException(
                DotItemException<CharT>{ text_iterator, error_message, error_suggestion });
            throw UnrecoverableError{ "unrecoverable error in DotItemType" };
        }

        text::TextIterator<CharT> &text_iterator;
    };
}// namespace cerb::lex::dot_item

#endif /*  CERBERUS_PROJECT_ITEMS_COUNTER_HPP */
