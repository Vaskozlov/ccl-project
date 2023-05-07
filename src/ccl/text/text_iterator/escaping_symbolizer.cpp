#include <ccl/text/text_iterator.hpp>

namespace ccl::text
{
    using EscapingSymbolizer = TextIterator::EscapingSequenceToChar;

    EscapingSymbolizer::EscapingSequenceToChar(
        TextIterator &text_iterator,
        extra_symbols_t extra_symbols) noexcept
      : extraSymbols{std::move(extra_symbols)}
      , textIterator{text_iterator}
    {}

    auto EscapingSymbolizer::matchNextChar() -> char32_t
    {
        CCL_ASSERT_MSG(textIterator.getCurrentChar() == '\\', "called without preceding `\\`");

        auto chr = textIterator.next();

        switch (chr) {
        case U'\\':
            return U'\\';

        case U'\'':
            return U'\'';

        case U'\"':
            return U'\"';

        case U'n':
            return U'\n';

        case U't':
            return U'\t';

        case U'r':
            return U'\r';

        case U'f':
            return U'\f';

        case U'b':
            return U'\b';

        case U'v':
            return U'\v';

        case U'a':
            return U'\a';

        case U'0':
            return calculateNotationEscapeSymbol(textIterator, 2, 3, false);

        case U'x':
            return calculateNotationEscapeSymbol(textIterator, 2, 4, true);

        case U'u':
            return calculateNotationEscapeSymbol(textIterator, 4, 4, true);

        case U'U':
            return calculateNotationEscapeSymbol(textIterator, 8, 4, true);// NOLINT

        default:
            return searchInExtraSymbols(chr);
        }
    }

    auto EscapingSymbolizer::searchInExtraSymbols(char32_t chr) -> char32_t
    {
        auto it = std::find_if(extraSymbols.cbegin(), extraSymbols.cend(), [chr](auto elem) {
            return elem.first == chr;
        });

        if (it == extraSymbols.end()) [[unlikely]] {
            throwUnableToMatchEscapingSymbol();
            return U'?';
        }

        return it->second;
    }

    auto EscapingSymbolizer::throwUnableToMatchEscapingSymbol() -> void
    {
        textIterator.throwUncriticalError(
            AnalysisStage::LEXICAL_ANALYSIS, "unable to matchNextChar any escaping symbol");
    }
}// namespace ccl::text
