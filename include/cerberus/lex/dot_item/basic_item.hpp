#ifndef CERBERUS_PROJECT_BASIC_ITEM_HPP
#define CERBERUS_PROJECT_BASIC_ITEM_HPP

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
        auto operator=(const BasicItem &) -> BasicItem & = default;
        auto operator=(BasicItem &&) noexcept -> BasicItem & = default;

        BasicItem() = default;
        BasicItem(BasicItem &&) noexcept = default;
        BasicItem(const BasicItem &) = default;

        virtual ~BasicItem() = default;
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_BASIC_ITEM_HPP */
