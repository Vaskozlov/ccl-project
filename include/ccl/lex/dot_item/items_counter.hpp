#ifndef CCL_PROJECT_ITEMS_COUNTER_HPP
#define CCL_PROJECT_ITEMS_COUNTER_HPP

#include <ccl/format/format.hpp>
#include <ccl/text/text_iterator.hpp>

namespace ccl::lex::dot_item
{
    CCL_EXCEPTION(DotItemException, text::TextIteratorException);

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

        struct CommentType : std::true_type
        {
        };

        constexpr auto Union = UnionType{};
        constexpr auto Sequence = SequenceType{};
        constexpr auto DotItem = DotItemType{};
        constexpr auto String = StringType{};
        constexpr auto Character = CharacterType{};
        constexpr auto Terminal = TerminalType{};
        constexpr auto Comment = CommentType{};
    }// namespace item

    struct ItemsCounter
    {
        ItemsCounter(ItemsCounter &&) = delete;
        ItemsCounter(const ItemsCounter &) = delete;

        explicit ItemsCounter(text::TextIterator &text_iterator_) noexcept
          : text_iterator(text_iterator_)
        {}

        ~ItemsCounter() = default;

        auto operator=(ItemsCounter &&) -> void = delete;
        auto operator=(const ItemsCounter &) -> void = delete;

        [[nodiscard]] auto hasStrings() const noexcept -> bool
        {
            return strings != 0;
        }

        [[nodiscard]] auto hasCharacters() const noexcept -> bool
        {
            return characters != 0;
        }

        [[nodiscard]] auto hasUnions() const noexcept -> bool
        {
            return unions != 0;
        }

        [[nodiscard]] auto hasSequences() const noexcept -> bool
        {
            return sequences != 0;
        }

        [[nodiscard]] auto hasDotItems() const noexcept -> bool
        {
            return dot_items != 0;
        }

        [[nodiscard]] auto hasTerminals() const noexcept -> bool
        {
            return special_tokens != 0;
        }

        [[nodiscard]] auto hasStrOrChar() const noexcept -> bool
        {
            return lor(hasStrings(), hasCharacters());
        }
        auto add(item::UnionType /* unused */) -> void;
        auto add(item::DotItemType /* unused */) -> void;
        auto add(item::SequenceType /* unused */) -> void;
        auto add(item::StringType /* unused */) -> void;
        auto add(item::CharacterType /* unused */) -> void;
        auto add(item::TerminalType /* unused */) -> void;
        auto add(item::CommentType /* unused */) -> void;

    private:
        auto checkAbilityToCreateSequence() -> void;
        auto checkAbilityToCreateTerminal() -> void;

        template<ConstString ItemName>
        auto checkThereIsOneSequence() -> void;

        template<ConstString ItemName>
        CCL_INLINE auto checkForUnexpectedSpecialSymbols() -> void;

        template<ConstString ItemName>
        CCL_INLINE auto checkNoStringOrChars() -> void;

        template<ConstString ItemName, ConstString Message, ConstString Suggestion>
        CCL_INLINE auto throwItemCreationError() -> void;

    public:
        size_t strings{};
        size_t characters{};
        size_t unions{};
        size_t sequences{};
        size_t dot_items{};
        size_t special_tokens{};

    private:
        text::TextIterator &text_iterator;
    };
}// namespace ccl::lex::dot_item

#endif /*  CCL_PROJECT_ITEMS_COUNTER_HPP */
