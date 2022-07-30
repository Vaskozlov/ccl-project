#ifndef CERBERUS_PROJECT_UNION_HPP
#define CERBERUS_PROJECT_UNION_HPP

#include <cerberus/lex/dot_item/basic_item.hpp>
#include <cerberus/typed_bitset.hpp>

namespace cerb::lex::dot_item
{
    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(UnionException, text::TextIteratorException<CharT>);

    template<CharacterLiteral CharT>
    class Union : public BasicItem<CharT>
    {
    public:
        using Base = BasicItem<CharT>;
        using typename Base::CommentTokens;
        using typename Base::ExceptionAccumulator;
        using typename Base::ScanStatus;
        using typename Base::TextIterator;

        CERBLIB_DECL auto scan(TextIterator &text_iterator) const -> ScanStatus override
        {
            auto times = static_cast<size_t>(0);

            while (true) {
                if (matching(text_iterator)) {
                    ++times;
                    text_iterator.nextRawChar();
                    continue;
                }

                return Base::repetition.inRange(times) ? ScanStatus::SUCCESS : ScanStatus::FAILURE;
            }
        }

        CERBLIB_DECL auto get() const -> const TypedBitset<CharT> &
        {
            return bitset;
        }

        constexpr explicit Union(TextIterator &rule_iterator_)
        {
            auto is_range = false;
            auto previous_chr = static_cast<CharT>(0);
            auto &rule_iterator = rule_iterator_;
            auto begin_iterator_state = rule_iterator;

            checkUnionBegin(rule_iterator);

            while (true) {
                auto [is_escaping, chr] = rule_iterator.nextRawCharWithEscapingSymbols();

                checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

                if (isUnionEnd(is_escaping, chr)) {
                    break;
                }

                if (isRange(is_range, chr)) {
                    is_range = true;
                    continue;
                }

                if (is_range) {
                    bitset.set(previous_chr, chr, true);
                    is_range = false;
                } else {
                    bitset.set(chr, true);
                }

                previous_chr = chr;
            }

            checkForClosedRange(rule_iterator, is_range);
            addWarningIfEmpty(rule_iterator, previous_chr == 0);
        }

        CERBLIB_DERIVED_CONSTRUCTORS(Union);

    private:
        CERBLIB_DECL auto matching(const TextIterator &text_iterator) const -> bool
        {
            return bitset.at(text_iterator.futureRawChar(1));
        }

        CERBLIB_DECL static auto isRange(bool is_escaping, CharT chr) -> bool
        {
            if (is_escaping) {
                return false;
            }

            return chr == '-';
        }

        CERBLIB_DECL static auto isUnionEnd(bool is_escaping, CharT chr) -> bool
        {
            return land(not is_escaping, chr == ']');
        }

        constexpr static auto
            checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, CharT chr) -> void
        {
            if (land(not is_escaping, isEoF(chr))) {
                throwUnterminatedUnion(rule_iterator);
            }
        }

        constexpr static auto checkUnionBegin(TextIterator &rule_iterator) -> void
        {
            if (rule_iterator.getCurrentChar() != '[') {
                throwUnionBeginException(rule_iterator);
            }
        }

        constexpr static auto checkForClosedRange(TextIterator &rule_iterator, bool is_open) -> void
        {
            if (is_open) {
                throwUnterminatedRangeException(rule_iterator);
            }
        }

        constexpr static auto addWarningIfEmpty(TextIterator &rule_iterator, bool is_empty) -> void
        {
            using namespace string_view_literals;

            if (is_empty) {
                rule_iterator.throwWarning(
                    UnionException<CharT>(rule_iterator, "empty union should not be used"_sv));
            }
        }

        constexpr static auto throwUnterminatedUnion(TextIterator &rule_iterator) -> void
        {
            using namespace string_view_literals;

            rule_iterator.throwException(
                UnionException<CharT>(rule_iterator, "unterminated union item"_sv));
            throw UnrecoverableError{ "unrecoverable error in Union" };
        }

        constexpr static auto throwUnterminatedRangeException(TextIterator &rule_iterator) -> void
        {
            using namespace string_view_literals;

            rule_iterator.throwException(
                UnionException<CharT>(rule_iterator, "unterminated range"_sv));
            throw UnrecoverableError{ "unrecoverable error in Union" };
        }

        constexpr static auto throwUnionBeginException(TextIterator &rule_iterator) -> void
        {
            auto message = fmt::
                format<CharT, "expected `[` at the beginning of union item declaration, got {}">(
                    rule_iterator.getCurrentChar());

            rule_iterator.throwException(UnionException<CharT>(rule_iterator, message));
            throw UnrecoverableError{ "unrecoverable error in Union" };
        }

        TypedBitset<CharT> bitset{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_UNION_HPP */
