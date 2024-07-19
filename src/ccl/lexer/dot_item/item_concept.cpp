#include <ccl/lexer/dot_item/item_concept.hpp>
#include <ccl/lexer/dot_item/scanner_template.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/string_node.hpp>

namespace ccl::lexer::dot_item
{
    class LexerScanner : public CrtpScanner<LexerScanner, ScanResult>
    {
    public:
        using CrtpScanner<LexerScanner, ScanResult>::CrtpScanner;

        static auto onIteration(const ScanResult & /* result */) noexcept -> void
        {
            // do nothing
        }

        [[nodiscard]] auto scanIteration(const DotItemConcept &item_concept) const -> ScanResult
        {
            return item_concept.scanIteration(textIterator);
        }

        [[nodiscard]] static auto constructResult(std::size_t totally_skipped) -> ScanResult
        {
            return ScanResult{totally_skipped};
        }
    };

    class ParserScanner : public CrtpScanner<ParserScanner, ParsingResult>
    {
    public:
        using CrtpScanner<ParserScanner, ParsingResult>::CrtpScanner;

    private:
        isl::UniquePtr<parser::ast::UnNodeSequence> nodeSequence =
            isl::makeUnique<parser::ast::UnNodeSequence>(item.getId());
        isl::string_view textBegin = textIterator.getRemaining();

    public:
        auto onIteration(ParsingResult &result) -> void
        {
            if (result.getNode() != nullptr) {
                nodeSequence->joinWithNode(result.getAndReleaseNode());
            }
        }

        [[nodiscard]] auto scanIteration(const DotItemConcept &item_concept) const -> ParsingResult
        {
            return item_concept.parseIteration(textIterator);
        }

        [[nodiscard]] auto constructResult(std::size_t totally_skipped) -> ParsingResult
        {
            if (nodeSequence->empty()) {
                const auto node_repr =
                    isl::string_view{textBegin.begin(), textIterator.getRemainingAsCarriage()};

                return ParsingResult{
                    totally_skipped,
                    isl::makeUnique<parser::ast::StringNode>(item.getId(), node_repr)};
            }

            return ParsingResult{totally_skipped, std::move(nodeSequence)};
        }
    };

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
        return LexerScanner(closure, *this, text_iterator).scan();
    }

    auto DotItemConcept::parse(ForkedGenerator text_iterator) const -> ParsingResult
    {
        return ParserScanner(closure, *this, text_iterator).scan();
    }
}// namespace ccl::lexer::dot_item
