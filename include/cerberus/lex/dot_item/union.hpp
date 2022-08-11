#ifndef CERBERUS_PROJECT_UNION_HPP
#define CERBERUS_PROJECT_UNION_HPP

#include <cerberus/lex/dot_item/basic_item.hpp>
#include <cerberus/utf_set.hpp>

namespace cerb::lex::dot_item
{
    CERBLIB_EXCEPTION(UnionException, text::TextIteratorException);

    class Union : public BasicItem
    {
    public:
        using BasicItem::isNextCharNotForScanning;

        using typename BasicItem::CommentTokens;
        using typename BasicItem::ExceptionAccumulator;
        using typename BasicItem::ScanStatus;
        using typename BasicItem::TextIterator;

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return bitset.empty();
        }

        [[nodiscard]] auto get() const -> const UtfSet &
        {
            return bitset;
        }

        Union(TextIterator &rule_iterator_, AnalysisShared &analysis_shared_)
          : BasicItem{ analysis_shared_ }
        {
            auto is_range = false;
            auto previous_chr = static_cast<char32_t>(0);
            auto &rule_iterator = rule_iterator_;
            auto begin_iterator_state = rule_iterator;

            checkUnionBegin(rule_iterator);

            while (true) {
                auto [is_escaping, chr] = rule_iterator.nextRawCharWithEscapingSymbols(
                    { { U'[', U'[' }, { U']', U']' }, { U'-', U'-' } });

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
        }

    private:
        [[nodiscard]] auto scanIteration(TextIterator &text_iterator) const -> bool override
        {
            return bitset.at(text_iterator.nextRawChar());
        }

        [[nodiscard]] static auto isRange(bool is_escaping, char32_t chr) -> bool
        {
            if (is_escaping) {
                return false;
            }

            return chr == U'-';
        }

        [[nodiscard]] static auto isUnionEnd(bool is_escaping, char32_t chr) -> bool
        {
            return land(not is_escaping, chr == U']');
        }

        static auto
            checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, char32_t chr)
                -> void
        {
            if (land(not is_escaping, isEoF(chr))) {
                throwUnterminatedUnion(rule_iterator);
            }
        }

        static auto checkUnionBegin(TextIterator &rule_iterator) -> void
        {
            if (rule_iterator.getCurrentChar() != U'[') {
                throwUnionBeginException(rule_iterator);
            }
        }

        static auto checkForClosedRange(TextIterator &rule_iterator, bool is_open) -> void
        {
            if (is_open) {
                throwUnterminatedRangeException(rule_iterator);
            }
        }

        static auto throwUnterminatedUnion(TextIterator &rule_iterator) -> void
        {
            using namespace std::string_view_literals;

            rule_iterator.template throwException<UnionException>(u8"unterminated union item"sv);
            throw UnrecoverableError{ "unrecoverable error in UnionType" };
        }

        static auto throwUnterminatedRangeException(TextIterator &rule_iterator) -> void
        {
            using namespace std::string_view_literals;

            rule_iterator.template throwException<UnionException>(u8"unterminated range"sv);
            throw UnrecoverableError{ "unrecoverable error in UnionType" };
        }

        static auto throwUnionBeginException(TextIterator &rule_iterator) -> void
        {
            auto message =
                fmt::format<u8"expected `[` at the beginning of union item declaration, got {}">(
                    rule_iterator.getCurrentChar());

            rule_iterator.template throwException<UnionException>(message);
            throw UnrecoverableError{ "unrecoverable error in UnionType" };
        }

        UtfSet bitset{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_UNION_HPP */
