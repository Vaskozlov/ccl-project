#include <ccl/lexer/rule/union.hpp>

namespace ccl::lexer::rule
{
    auto parseUnionDecl(const isl::string_view union_decl)
        -> isl::Pair<std::bitset<isl::UtfSet::asciiStorageSize>, std::vector<isl::Range<char32_t>>>
    {
        static constexpr isl::StaticFlatmap<char32_t, char32_t, 3> special_symbols = {
            {U'[', U'['},
            {U']', U']'},
            {U'-', U'-'},
        };

        const auto special_symbols_span = std::span{special_symbols.begin(), special_symbols.end()};

        auto text_iterator = text::TextIterator{union_decl};
        auto is_range = false;
        auto previous_chr = U'\0';
        [[maybe_unused]] auto skip_union_angle_open = text_iterator.advance();

        auto result = isl::Pair<
            std::bitset<isl::UtfSet::asciiStorageSize>, std::vector<isl::Range<char32_t>>>{};

        while (true) {
            auto [is_escaping, chr] =
                text_iterator.nextCharWithEscapingSymbols(special_symbols_span);

            if (!is_escaping && chr == U']') {
                break;
            }

            if (!is_escaping && chr == U'-' && !is_range) {
                is_range = true;
                continue;
            }

            if (is_range) {
                is_range = false;
                result.second.emplace_back(previous_chr, chr);
            } else {
                if (chr < isl::UtfSet::asciiStorageSize) {
                    result.first[chr] = true;
                } else {
                    result.second.emplace_back(chr, chr);
                }
            }

            previous_chr = chr;
        }

        return result;
    }

    Union::Union(TextIterator &rule_iterator)
    {
        static constexpr isl::StaticFlatmap<char32_t, char32_t, 3> special_symbols = {
            {U'[', U'['},
            {U']', U']'},
            {U'-', U'-'},
        };

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

    Union::Union(isl::UtfSet stored_symbols)
      : storedSymbols{std::move(stored_symbols)}
    {}

    auto Union::scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult
    {
        return storedSymbols.at(text_iterator.futureChar()) != isReversed()
                   ? ScanResult{isl::utf8::size(text_iterator.getNextCarriageValue())}
                   : ScanResult::failure();
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
            storedSymbols.set({previous_chr, chr + 1}, true);
            is_range = false;
        } else {
            storedSymbols.set(chr, true);
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
}// namespace ccl::lexer::rule
