#include <ccl/lexer/dot_item/item_concept.hpp>

namespace ccl::lexer::dot_item
{
    auto DotItemConcept::alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition)
        -> void
    {
        if (condition) [[unlikely]] {
            text_iterator.throwWarning(
                AnalysisStage::LEXICAL_ANALYSIS, "item will be always recognized");
        }
    }

    auto DotItemConcept::neverRecognizedSuggestion(TextIterator &text_iterator, bool condition)
        -> void
    {
        if (condition) [[unlikely]] {
            text_iterator.throwWarning(
                AnalysisStage::LEXICAL_ANALYSIS, "item will never be recognized");
        }
    }

    auto DotItemConcept::scan(ForkedGenerator text_iterator) const -> ScanResult
    {
        auto times = isl::as<std::size_t>(0);
        auto totally_skipped = isl::as<std::size_t>(0);

        while (!text_iterator.isEOI() && times < repetition.to) {
            auto chars_to_skip = scanIteration(text_iterator);

            if (chars_to_skip.isFailure()) {
                break;
            }

            text_iterator.skip(chars_to_skip.getBytesCount());
            totally_skipped += chars_to_skip.getBytesCount();
            ++times;
        }

        if (repetition.isInClosure(times)) {
            return ScanResult{totally_skipped};
        }

        return ScanResult::failure();
    }
}// namespace ccl::lexer::dot_item
