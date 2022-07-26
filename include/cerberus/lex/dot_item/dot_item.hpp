#ifndef CERBERUS_PROJECT_DOT_ITEM_HPP
#define CERBERUS_PROJECT_DOT_ITEM_HPP

#include <boost/container/small_vector.hpp>
#include <cerberus/lex/dot_item/string.hpp>
#include <cerberus/lex/dot_item/union.hpp>

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
            parseRule(rule_iterator);
        }

        constexpr explicit DotItem(TextIterator &rule_iterator_)
        {
            parseRule(rule_iterator_);
        }

        constexpr explicit DotItem(TextIterator &&rule_iterator_)
        {
            parseRule(rule_iterator_);
        }

        CERBLIB_DERIVED_CONSTRUCTORS(DotItem);

    private:
        constexpr auto parseRule(TextIterator &rule_iterator) -> void
        {
            while (not isEoF(rule_iterator.nextCleanChar())) {
                auto chr = rule_iterator.getCurrentChar();
                recognizeAction(chr, rule_iterator);
            }
        }

        constexpr auto recognizeAction(CharT chr, TextIterator &rule_iterator) -> void
        {
            switch (chr) {
            case '[':
                items.emplace_back(constructNewUnion(rule_iterator));
                break;

            case '\"':
                items.emplace_back(constructNewString(rule_iterator));
                break;

            case '(':
                items.emplace_back(constructNewItem(rule_iterator));
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

            case 'c':// for chars
                break;

            case 's':// for strings
                break;

            case 'm':// for multiline strings
                break;

            default:
                throwUndefinedAction(rule_iterator);
                break;
            }
        }

        CERBLIB_DECL static auto constructNewString(TextIterator &rule_iterator)
            -> std::unique_ptr<BasicItem<CharT>>
        {
            return std::make_unique<String<CharT>>(false, "\"", rule_iterator);
        }

        CERBLIB_DECL static auto constructNewUnion(TextIterator &rule_iterator)
            -> std::unique_ptr<BasicItem<CharT>>
        {
            return std::make_unique<Union<CharT>>(rule_iterator);
        }

        CERBLIB_DECL static auto constructNewItem(TextIterator &rule_iterator)
            -> std::unique_ptr<BasicItem<CharT>>
        {
            auto text = rule_iterator.getRemaining();
            auto saved_end = text.end();
            auto bracket_index = text.openCloseFind('(', ')');

            if (bracket_index == StrView<CharT>::npos) {
                throwUnterminatedDotItem(rule_iterator);
            }

            rule_iterator.setEnd(text.begin() + bracket_index);

            auto new_dot_item = std::make_unique<DotItem<CharT>>(rule_iterator);
            rule_iterator.setEnd(saved_end);

            return new_dot_item;
        }

        constexpr static auto throwUnterminatedDotItem(TextIterator &rule_iterator) -> void
        {
            auto error = DotItemException<CharT>(rule_iterator, "unterminated dot item");
            rule_iterator.throwException(std::move(error));
        }

        constexpr static auto throwUndefinedAction(TextIterator &rule_iterator) -> void
        {
            auto error = DotItemException<CharT>(
                rule_iterator, "undefined action",
                "Use `\"` for string and `\'` for terminal symbol");

            rule_iterator.throwException(std::move(error));
        }

        boost::container::small_vector<std::unique_ptr<BasicItem<CharT>>, 4> items{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_HPP */
