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

        auto operator+=(item::UnionType /* unused */) -> ItemsCounter &;
        auto operator+=(item::DotItemType /* unused */) -> ItemsCounter &;
        auto operator+=(item::SequenceType /* unused */) -> ItemsCounter &;
        auto operator+=(item::StringType /* unused */) -> ItemsCounter &;
        auto operator+=(item::CharacterType /* unused */) -> ItemsCounter &;
        auto operator+=(item::TerminalType /* unused */) -> ItemsCounter &;

        explicit ItemsCounter(text::TextIterator &text_iterator_) : text_iterator(text_iterator_)
        {}

    private:
        auto checkAbilityToCreateSequence() -> void;
        auto checkAbilityToCreateTerminal() -> void;
        auto checkAbilityToCreateStringOrCharacter() -> void;
        auto checkForUnexpectedTerminals(std::u8string_view item_name) -> void;
        auto checkNoStringOrChars(std::u8string_view item_name) -> void;

        auto throwItemCreationError(
            std::u8string_view item_name,
            std::u8string_view message,
            std::u8string_view suggestion = {}) -> void;

    public:
        size_t strings{};
        size_t characters{};
        size_t unions{};
        size_t sequences{};
        size_t dot_items{};
        size_t terminals{};

    private:
        text::TextIterator &text_iterator;
    };
}// namespace cerb::lex::dot_item

#endif /*  CERBERUS_PROJECT_ITEMS_COUNTER_HPP */
