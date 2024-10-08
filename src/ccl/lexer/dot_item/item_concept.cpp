#include <ccl/lexer/rule/rule_block_interface.hpp>
#include <ccl/lexer/rule/scanner_template.hpp>

namespace ccl::lexer::rule
{
    class LexerScanner : public CrtpScanner<LexerScanner, ScanResult>
    {
    public:
        using CrtpScanner::CrtpScanner;

        static auto onIteration(const ScanResult & /* result */) noexcept -> void
        {
            // do nothing
        }

        [[nodiscard]] auto scanIteration(const RuleBlockInterface &item_concept) const -> ScanResult
        {
            return item_concept.scanIteration(textIterator);
        }

        [[nodiscard]] static auto constructResult(const std::size_t totally_skipped) -> ScanResult
        {
            return ScanResult{totally_skipped};
        }
    };

    auto RuleBlockInterface::alwaysRecognizedSuggestion(
        const TextIterator &text_iterator, const bool condition) -> void
    {
        if (condition) [[unlikely]] {
            text_iterator.throwWarning(
                AnalysisStage::LEXICAL_ANALYSIS, "item will be always recognized");
        }
    }

    auto RuleBlockInterface::neverRecognizedSuggestion(
        const TextIterator &text_iterator, const bool condition) -> void
    {
        if (condition) [[unlikely]] {
            text_iterator.throwWarning(
                AnalysisStage::LEXICAL_ANALYSIS, "item will never be recognized");
        }
    }

    auto RuleBlockInterface::scan(ForkedGenerator text_iterator) const -> ScanResult
    {
        return LexerScanner(repetition, *this, text_iterator).scan();
    }
}// namespace ccl::lexer::rule
