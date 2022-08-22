#include <cerberus/lex/dot_item/union.hpp>

namespace cerb::lex::dot_item
{
    using namespace cerb::string_view_literals;

    Union::Union(TextIterator &rule_iterator_, AnalysisShared &analysis_shared_)
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

            addCharactersToTheBitset(is_range, previous_chr, chr);
            previous_chr = chr;
        }

        checkForClosedRange(rule_iterator, is_range);
    }

    auto Union::empty() const noexcept -> bool
    {
        return bitset.empty();
    }

    auto Union::scanIteration(TextIterator &text_iterator, Token & /* unused */) const -> bool
    {
        return bitset.at(text_iterator.nextRawChar());
    }

    auto Union::isRange(bool is_escaping, char32_t chr) noexcept -> bool
    {
        if (is_escaping) {
            return false;
        }

        return chr == U'-';
    }

    auto Union::isUnionEnd(bool is_escaping, char32_t chr) noexcept -> bool
    {
        return land(not is_escaping, chr == U']');
    }

    auto Union::addCharactersToTheBitset(bool &is_range, char32_t first, char32_t last) -> void
    {
        if (is_range) {
            bitset.set(first, last, true);
            is_range = false;
        } else {
            bitset.set(first, true);
        }
    }

    auto Union::checkForUnexpectedEnd(TextIterator &rule_iterator, bool is_escaping, char32_t chr)
        -> void
    {
        if (land(not is_escaping, isEoF(chr))) {
            throwUnterminatedUnion(rule_iterator);
        }
    }

    auto Union::checkUnionBegin(TextIterator &rule_iterator) -> void
    {
        if (rule_iterator.getCurrentChar() != U'[') {
            throwUnionBeginException(rule_iterator);
        }
    }

    auto Union::checkForClosedRange(TextIterator &rule_iterator, bool is_open) -> void
    {
        if (is_open) {
            throwUnterminatedRangeException(rule_iterator);
        }
    }

    auto Union::throwUnterminatedUnion(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwException<UnionException>(u8"unterminated union item"_sv);
        throw UnrecoverableError{ "unrecoverable error in Union" };
    }

    auto Union::throwUnterminatedRangeException(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwException<UnionException>(u8"unterminated range"_sv);
        throw UnrecoverableError{ "unrecoverable error in Union" };
    }

    auto Union::throwUnionBeginException(TextIterator &rule_iterator) -> void
    {
        auto message =
            fmt::format<u8"expected `[` at the beginning of union item declaration, got {}">(
                rule_iterator.getCurrentChar());

        rule_iterator.throwException<UnionException>(message);
        throw UnrecoverableError{ "unrecoverable error in Union" };
    }
}// namespace cerb::lex::dot_item
