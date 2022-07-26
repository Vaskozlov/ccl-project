#ifndef CERBERUS_PROJECT_BASIC_ITEM_HPP
#define CERBERUS_PROJECT_BASIC_ITEM_HPP

#include <cerberus/lex/typedefs.hpp>
#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    struct CERBLIB_TRIVIAL_ABI Repetition
    {
        consteval static auto basic() -> Repetition
        {
            return { 1, 1 };
        }

        consteval static auto question() -> Repetition
        {
            return { 0, 1 };
        }

        consteval static auto star() -> Repetition
        {
            return { 0, std::numeric_limits<size_t>::max() };
        }

        consteval static auto plus() -> Repetition
        {
            return { 1, std::numeric_limits<size_t>::max() };
        }

        size_t from;
        size_t to;
    };

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

    protected:
        Repetition repetition{ Repetition::basic() };
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_BASIC_ITEM_HPP */
