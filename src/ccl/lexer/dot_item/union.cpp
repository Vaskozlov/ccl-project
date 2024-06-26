#include <ccl/lexer/dot_item/union.hpp>
#include <ccl/parser/ast/value.hpp>

namespace ccl::lexer::dot_item
{
    Union::Union(TextIterator &rule_iterator, Id rule_id)
      : DotItemConcept{rule_id}
    {
        static constexpr isl::StaticFlatmap<char32_t, char32_t, 3> special_symbols = {
            {U'[', U'['}, {U']', U']'}, {U'-', U'-'}};

        auto is_range = false;
        auto previous_chr = U'\0';
        auto begin_iterator_state = rule_iterator;

        checkUnionBegin(rule_iterator);

        while (true) {
            auto [is_escaping, chr] = rule_iterator.nextCharWithEscapingSymbols(
                std::span{special_symbols.begin(), special_symbols.end()});

            checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

            if (isUnionEnd(is_escaping, chr)) {
                break;
            }

            if (isRange(is_escaping, chr)) {
                is_range = true;
                continue;
            }

            addCharactersToTheBitset(is_range, previous_chr, chr);
            previous_chr = chr;
        }

        checkForClosedRange(rule_iterator, is_range);
    }

    auto Union::scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult
    {
        return bitset.at(text_iterator.futureChar()) != isReversed()
                   ? ScanResult{isl::utf8::size(text_iterator.getNextCarriageValue())}
                   : ScanResult::failure();
    }

    auto Union::parseIteration(const ForkedGenerator &text_iterator) const -> ParsingResult
    {
        const auto result = scanIteration(text_iterator);

        if (result.isFailure()) {
            return ParsingResult::failure();
        }

        return ParsingResult{result.getBytesCount(), nullptr};
    }

    CCL_INLINE auto Union::isRange(bool is_escaping, char32_t chr) noexcept -> bool
    {
        if (is_escaping) {
            return false;
        }

        return chr == U'-';
    }

    CCL_INLINE auto Union::isUnionEnd(bool is_escaping, char32_t chr) noexcept -> bool
    {
        return !is_escaping && chr == U']';
    }

    CCL_INLINE auto
        Union::addCharactersToTheBitset(bool &is_range, char32_t previous_chr, char32_t chr) -> void
    {
        if (is_range) {
            bitset.set({previous_chr, chr + 1}, true);
            is_range = false;
        } else {
            bitset.set(chr, true);
        }
    }

    CCL_INLINE auto Union::checkForUnexpectedEnd(
        TextIterator &rule_iterator, bool is_escaping, char32_t chr) -> void
    {
        if (!is_escaping && isEoF(chr)) {
            throwUnterminatedUnion(rule_iterator);
        }
    }

    CCL_INLINE auto Union::checkUnionBegin(TextIterator &rule_iterator) -> void
    {
        if (U'[' != rule_iterator.getCurrentChar()) {
            throwUnionBeginException(rule_iterator);
        }
    }

    CCL_INLINE auto
        Union::checkForClosedRange(TextIterator &rule_iterator, bool is_ranged_opened) -> void
    {
        if (is_ranged_opened) {
            throwUnterminatedRangeException(rule_iterator);
        }
    }

    CCL_INLINE auto Union::throwUnterminatedUnion(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, "unterminated union item");
        throw UnrecoverableError{"unrecoverable error in Union"};
    }

    CCL_INLINE auto Union::throwUnterminatedRangeException(TextIterator &rule_iterator) -> void
    {
        rule_iterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, "unterminated range");
        throw UnrecoverableError{"unrecoverable error in Union"};
    }

    CCL_INLINE auto Union::throwUnionBeginException(TextIterator &rule_iterator) -> void
    {
        auto buffer = std::string{};
        isl::utf8::appendUtf32ToUtf8Container(buffer, rule_iterator.getCurrentChar());

        auto message =
            fmt::format("expected `[` at the beginning of union item declaration, got {}", buffer);

        rule_iterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message);
        throw UnrecoverableError{"unrecoverable error in Union"};
    }
}// namespace ccl::lexer::dot_item
