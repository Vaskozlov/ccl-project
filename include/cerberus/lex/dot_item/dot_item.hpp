#ifndef CERBERUS_PROJECT_DOT_ITEM_HPP
#define CERBERUS_PROJECT_DOT_ITEM_HPP

#include <boost/container/small_vector.hpp>
#include <cerberus/lex/dot_item/string.hpp>
#include <cerberus/lex/dot_item/union.hpp>

namespace cerb::lex::dot_item
{
    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(DotItemException, text::TextIteratorException<CharT>);

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
                addRepetition(rule_iterator, Repetition::star());
                break;

            case '+':
                addRepetition(rule_iterator, Repetition::plus());
                break;

            case '?':
                addRepetition(rule_iterator, Repetition::question());
                break;

            case '{':
                addRepetition(rule_iterator, Repetition{ rule_iterator });
                break;

            case '^':
                makeReverse(rule_iterator);
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

        constexpr auto addRepetition(TextIterator &rule_iterator, Repetition new_repetition) -> void
        {
            if (items.size() == 0) {
                throwUnableToApply(
                    rule_iterator, "no item to apply repetition", "set repetition after item");
            }

            auto &last_item = items.back();

            if (last_item->getRepetition() != Repetition::basic()) {
                throwUnableToApply(
                    rule_iterator, "item already has repetition", "do not set repetition twice");
            }

            last_item->setRepetition(new_repetition);
        }

        constexpr auto makeReverse(TextIterator &rule_iterator) -> void
        {
            if (items.size() == 0) {
                throwUnableToApply(
                    rule_iterator, "no item to reverse", "create item before reversing it");
            }

            auto &last_item = items.back();

            if (last_item->isReversed()) {
                throwUnableToApply(
                    rule_iterator, "item is already reversed", "do not set reverse for item twice");
            }

            last_item->makeReverse();
        }

        constexpr static auto throwUnableToApply(
            TextIterator &rule_iterator, string_view reason, string_view suggestion) -> void
        {
            auto message = fmt::format<CharT, "unable tp apply with reason: {}">(reason);
            auto converted_suggestion = fmt::format<CharT, "{}">(suggestion);
            rule_iterator.throwException(
                DotItemException<CharT>(rule_iterator, message, converted_suggestion));
        }

        constexpr static auto throwUnterminatedDotItem(TextIterator &rule_iterator) -> void
        {
            rule_iterator.throwException(
                DotItemException<CharT>(rule_iterator, "unterminated dot item"));
        }

        constexpr static auto throwUndefinedAction(TextIterator &rule_iterator) -> void
        {
            const auto *message = "undefined action";
            const auto *suggestion = "Use `\"` for string and `\'` for terminal symbol";

            rule_iterator.throwException(
                DotItemException<CharT>(rule_iterator, message, suggestion));
        }

        boost::container::small_vector<std::unique_ptr<BasicItem<CharT>>, 4> items{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_HPP */
