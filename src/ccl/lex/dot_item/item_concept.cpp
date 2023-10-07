#include <ccl/lex/dot_item/item_concept.hpp>

using namespace ccl::integral_literals;

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

    auto DotItemConcept::scan(ForkedGenerator text_iterator) const -> std::optional<size_t>
    {
        auto times = as<size_t>(0);
        auto totally_skipped = as<size_t>(0);

        while (!text_iterator.isEOI() && times < repetition.to) {
            auto chars_to_skip = scanIteration(text_iterator);

            if (!chars_to_skip.has_value()) {
                break;
            }

            text_iterator.skip(*chars_to_skip);
            totally_skipped += *chars_to_skip;
            ++times;
        }

        if (repetition.inRange(times)) {
            return totally_skipped;
        }

        return std::nullopt;
    }
}// namespace ccl::lex::dot_item
