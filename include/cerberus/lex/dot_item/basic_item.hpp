#ifndef CERBERUS_PROJECT_BASIC_ITEM_HPP
#define CERBERUS_PROJECT_BASIC_ITEM_HPP

#include <cerberus/lex/dot_item/repetition.hpp>
#include <cerberus/lex/typedefs.hpp>
#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    template<CharacterLiteral CharT>
    class BasicItem
    {
    protected:
        using TextIterator = text::TextIterator<CharT>;
        using CommentTokens = text::module::CommentTokens<CharT>;
        using ExceptionAccumulator =
            analysis::ExceptionAccumulator<text::TextIteratorException<CharT>>;

    public:
        CERBLIB_DECL auto getRepetition() const -> Repetition
        {
            return repetition;
        }

        CERBLIB_DECL auto isReversed() const -> bool
        {
            return reversed;
        }

        constexpr auto makeReverse() -> void
        {
            reversed = true;
        }

        constexpr auto setRepetition(Repetition new_repetition) -> void
        {
            repetition = new_repetition;
        }

        auto operator=(const BasicItem &) -> BasicItem & = default;
        auto operator=(BasicItem &&) noexcept -> BasicItem & = default;

        BasicItem() = default;
        BasicItem(BasicItem &&) noexcept = default;
        BasicItem(const BasicItem &) = default;

        virtual ~BasicItem() = default;

    protected:
        Repetition repetition{ Repetition::basic() };
        bool reversed{ false };
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_BASIC_ITEM_HPP */
