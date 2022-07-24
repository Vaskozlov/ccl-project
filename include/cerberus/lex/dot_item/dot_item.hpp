#ifndef CERBERUS_PROJECT_DOT_ITEM_HPP
#define CERBERUS_PROJECT_DOT_ITEM_HPP

#include <cerberus/lex/dot_item/string.hpp>

namespace cerb::lex::dot_item
{
    template<CharacterLiteral CharT>
    using ExceptionAccumulator = analysis::ExceptionAccumulator<text::TextIteratorException<CharT>>;

    template<CharacterLiteral CharT>
    using CommentTokens = text::module::CommentTokens<CharT>;

    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(DotItemException, text::TextIteratorException<CharT>);

    template<CharacterLiteral CharT>
    struct RuleInitializer
    {
        StrView<CharT> rule{};
        StrView<CharT> filename{};
        ExceptionAccumulator<CharT> *exception_accumulator{ nullptr };
        CommentTokens<CharT> comment_tokens{};
        ssize_t id{};
    };

    template<CharacterLiteral CharT>
    class DotItem : public BasicItem<CharT>
    {
    private:
        using Base = BasicItem<CharT>;
        using typename Base::CommentTokens;
        using typename Base::ExceptionAccumulator;
        using typename Base::TextIterator;

    public:
        constexpr explicit DotItem(const TextIterator &rule_iterator_)
        {
            auto rule_iterator = rule_iterator_;
            parseRule(std::move(rule_iterator));
        }

        constexpr explicit DotItem(TextIterator &&rule_iterator_)
        {
            parseRule(std::forward<TextIterator>(rule_iterator_));
        }

        ~DotItem() = default;

    private:
        constexpr auto parseRule(TextIterator &&rule_iterator) -> void
        {
            while (not isEoF(rule_iterator.nextCleanChar())) {
                auto chr = rule_iterator.getCurrentChar();
                recognizeAction(chr, rule_iterator);
            }
        }

        constexpr auto recognizeAction(CharT chr, TextIterator & /*rule_iterator*/) -> void
        {
            switch (chr) {
            case '[':

                break;

            case '\"':
                break;

            case '(':
                break;

            case '*':
                break;

            case '+':
                break;

            case '?':
                break;

            case '{':
                break;

            case '^':
                break;

            default:
                // throwUndefinedAction(rule_iterator);
                break;
            }
        }

        constexpr static auto throwUndefinedAction(TextIterator &rule_iterator) -> void
        {
            auto error = DotItemException<CharT>(
                rule_iterator, "undefined action",
                "Use '\" for string and '\' for terminal symbol");

            rule_iterator.throwException(std::move(error));
        }
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_HPP */
