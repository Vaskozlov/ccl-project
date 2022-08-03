#ifndef CERBERUS_PROJECT_DOT_ITEM_HPP
#define CERBERUS_PROJECT_DOT_ITEM_HPP

#include <boost/container/small_vector.hpp>
#include <cerberus/lex/analysis_shared.hpp>
#include <cerberus/lex/dot_item/sequence.hpp>
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

        using Base::analysis_shared;
        using Base::isNextCharNotForScanning;
        using Base::repetition;

        using typename Base::CommentTokens;
        using typename Base::ExceptionAccumulator;
        using typename Base::ScanStatus;
        using typename Base::TextIterator;

        struct ItemsCounter
        {
            size_t strings{};
            size_t unions{};
            size_t sequences{};
            size_t dot_items{};
            size_t terminals{};
        };

    public:
        CERBLIB_DECL auto getId() const -> size_t
        {
            return id;
        }

        CERBLIB_DECL auto empty() const -> bool override
        {
            return items.empty();
        }

        constexpr explicit DotItem(
            const TextIterator &rule_iterator_,
            size_t id_,
            AnalysisShared<CharT> &analysis_shared_)
          : Base(analysis_shared_), id(id_)
        {
            auto rule_iterator = rule_iterator_;
            parseRule(rule_iterator);
        }

        constexpr explicit DotItem(
            TextIterator &&rule_iterator_,
            size_t id_,
            AnalysisShared<CharT> &analysis_shared_)
          : Base(analysis_shared_), id(id_)
        {
            parseRule(rule_iterator_);
        }

        constexpr explicit DotItem(
            TextIterator &rule_iterator_,
            size_t id_,
            AnalysisShared<CharT> &analysis_shared_)
          : Base(analysis_shared_), id(id_)
        {
            parseRule(rule_iterator_);
        }

        CERBLIB_DERIVED_CONSTRUCTORS(DotItem);

    private:
        CERBLIB_DECL auto scanIteration(TextIterator &text_iterator) const -> bool override
        {
            for (auto &item : items) {
                if (auto [success, iterator] = item->scan(text_iterator); success) {
                    text_iterator = std::move(iterator);
                } else {
                    return false;
                }
            }

            return true;
        }

        constexpr auto parseRule(TextIterator &rule_iterator) -> void
        {
            auto counter = ItemsCounter{};
            rule_iterator.skipCommentsAndLayout();

            while (movedToTheNextChar(rule_iterator)) {
                auto chr = rule_iterator.getCurrentChar();
                recognizeAction(rule_iterator, chr, counter);
                rule_iterator.skipCommentsAndLayout();
            }
        }

        CERBLIB_DECL static auto movedToTheNextChar(TextIterator &rule_iterator) -> bool
        {
            return not isEoF(rule_iterator.nextRawChar());
        }

        constexpr auto
            recognizeAction(TextIterator &rule_iterator, CharT chr, ItemsCounter &counter) -> void
        {
            switch (chr) {
            case '[':
                ++counter.unions;
                emplaceItem(constructNewUnion(rule_iterator));
                break;

            case '\"':
                emplaceItem(constructNewSequence(rule_iterator));
                break;

            case '(':
                emplaceItem(constructNewItem(rule_iterator));
                break;

            case '\'':
                constructTerminal(rule_iterator);
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

            case 'c':// chars
                constructString(rule_iterator, true, false);
                break;

            case 's':// strings
                constructString(rule_iterator, false, false);
                break;

            case 'm':// multiline strings
                constructString(rule_iterator, false, true);
                break;

            default:
                throwUndefinedAction(rule_iterator);
                break;
            }
        }

        CERBLIB_DECL auto constructNewSequence(TextIterator &rule_iterator)
            -> std::unique_ptr<BasicItem<CharT>>
        {
            return std::make_unique<Sequence<CharT>>(false, "\"", rule_iterator, analysis_shared);
        }

        CERBLIB_DECL auto constructNewUnion(TextIterator &rule_iterator)
            -> std::unique_ptr<BasicItem<CharT>>
        {
            return std::make_unique<Union<CharT>>(rule_iterator, analysis_shared);
        }

        CERBLIB_DECL auto constructNewItem(TextIterator &rule_iterator)
            -> std::unique_ptr<BasicItem<CharT>>
        {
            auto text = rule_iterator.getRemaining();
            auto saved_end = text.end();
            auto bracket_index = text.openCloseFind('(', ')');

            if (bracket_index == StrView<CharT>::npos) {
                throwUnterminatedDotItem(rule_iterator);
            }

            rule_iterator.setEnd(text.begin() + bracket_index);

            auto new_dot_item =
                std::make_unique<DotItem<CharT>>(rule_iterator, id, analysis_shared);
            rule_iterator.setEnd(saved_end);

            return new_dot_item;
        }

        constexpr auto emplaceItem(std::unique_ptr<BasicItem<CharT>> item) -> void
        {
            if (not item->empty() || item->getRepetition().from != 0) {
                items.emplace_back(std::move(item));
            }
        }

        constexpr auto
            constructString(TextIterator &rule_iterator, bool is_character, bool is_multiline)
                -> void
        {
            if (items.empty() || dynamic_cast<Sequence<CharT> *>(items.back().get()) == nullptr) {
                throwUnableToApply(
                    rule_iterator,
                    "unable to apply char/string modifier, because there are not "
                    "any items or the last item is not a sequence",
                    "create sequence");
            }

            checkSize(
                rule_iterator, 1, "dot item with string must contain only one item - sequence",
                "delete other items");

            auto *sequence = dynamic_cast<Sequence<CharT> *>(items.back().get());
            auto &strings_and_chars = analysis_shared.strings_and_chars;

            strings_and_chars.emplace_back(
                std::move(sequence->getRef()), 0, is_character, is_multiline);

            items.pop_back();
        }

        constexpr auto constructTerminal(TextIterator &rule_iterator) -> void
        {
            checkSize(
                rule_iterator, 0, "dot item with terminal must be empty", "delete other items");

            auto sequence = Sequence<CharT>(false, "\'", rule_iterator, analysis_shared);
            auto &terminals = analysis_shared.terminals;

            terminals.addString(std::move(sequence.getRef()), id);
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

            last_item->reverse();
        }

        constexpr auto checkSize(
            TextIterator &rule_iterator, size_t expected_size, string_view message,
            string_view suggestion = {}) -> void
        {
            if (items.size() != expected_size) {
                throwUnexpectedSize(rule_iterator, message, suggestion);
            }
        }

        constexpr static auto throwUnexpectedSize(
            TextIterator &rule_iterator,
            string_view message,
            string_view suggestion) -> void
        {
            rule_iterator.throwException(
                DotItemException<CharT>(rule_iterator, message, suggestion));
            throw UnrecoverableError{ "unrecoverable error in dot item" };
        }

        constexpr static auto throwUnableToApply(
            TextIterator &rule_iterator,
            string_view reason,
            string_view suggestion) -> void
        {
            auto message = fmt::format<CharT, "unable to apply with reason: {}">(reason);
            auto converted_suggestion = fmt::format<CharT, "{}">(suggestion);

            rule_iterator.throwException(
                DotItemException<CharT>(rule_iterator, message, converted_suggestion));
            throw UnrecoverableError{ "unrecoverable error in DotItem" };
        }

        constexpr static auto throwUnterminatedDotItem(TextIterator &rule_iterator) -> void
        {
            rule_iterator.throwException(
                DotItemException<CharT>(rule_iterator, "unterminated dot item"));
            throw UnrecoverableError{ "unrecoverable error in DotItem" };
        }

        constexpr static auto throwUndefinedAction(TextIterator &rule_iterator) -> void
        {
            using namespace string_view_literals;

            constexpr auto message = "undefined action"_sv;
            constexpr auto suggestion =
                R"(Use `"` for string, `'` for terminal symbol and `[` for unions")"_sv;

            rule_iterator.throwException(
                DotItemException<CharT>(rule_iterator, message, suggestion));
            throw UnrecoverableError{ "unrecoverable error in DotItem" };
        }

        boost::container::small_vector<std::unique_ptr<BasicItem<CharT>>, 4> items{};
        size_t id{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_DOT_ITEM_HPP */
