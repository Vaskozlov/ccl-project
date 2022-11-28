#include <ccl/lex/dot_item/basic_item.hpp>

using namespace ccl::integral_literals;

namespace ccl::lex::dot_item
{
    auto BasicItem::alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
    {
        if (condition) [[unlikely]] {
            text_iterator.throwSuggestion(
                AnalysisStage::LEXICAL_ANALYSIS, "item will be always recognized", "delete it");
        }
    }

    auto BasicItem::neverRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
    {
        if (condition) [[unlikely]] {
            text_iterator.throwSuggestion(
                AnalysisStage::LEXICAL_ANALYSIS, "item will never be recognized");
        }
    }

    auto BasicItem::scan(ForkedGenerator text_iterator) const -> Optional<size_t>
    {
        auto times = as<size_t>(0);
        auto totally_skipped = as<size_t>(0);

        while (times < repetition.to) {
            if (text_iterator.isEOI()) {
                break;
            }

            auto scan_result = scanIteration(text_iterator);

            if (0 == scan_result) {
                break;
            }

            text_iterator.skip(scan_result);
            totally_skipped += scan_result;
            ++times;
        }

        if (repetition.inRange(times)) {
            return totally_skipped;
        }

        return std::nullopt;
    }
}// namespace ccl::lex::dot_item
