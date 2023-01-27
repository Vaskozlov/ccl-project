#include <ccl/lex/dot_item/basic_item.hpp>

using namespace ccl::integral_literals;

namespace ccl::lex::dot_item
{
    auto BasicItem::alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
    {
        if (condition) [[unlikely]] {
            text_iterator.throwWarning(
                AnalysisStage::LEXICAL_ANALYSIS, "item will be always recognized");
        }
    }

    auto BasicItem::neverRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
    {
        if (condition) [[unlikely]] {
            text_iterator.throwWarning(
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

            auto chars_to_skip = scanIteration(text_iterator);

            if (chars_to_skip == 0) {
                break;
            }

            text_iterator.skip(chars_to_skip);
            totally_skipped += chars_to_skip;
            ++times;
        }

        if (repetition.inRange(times)) {
            return totally_skipped;
        }

        return std::nullopt;
    }
}// namespace ccl::lex::dot_item
