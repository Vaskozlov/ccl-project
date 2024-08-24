#ifndef CCL_PROJECT_RULE_REFERENCE_HPP
#define CCL_PROJECT_RULE_REFERENCE_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/sequence.hpp>

namespace ccl::lexer::rule
{
    class RuleReference : public Sequence
    {
    private:
        // NOLINTNEXTLINE reference
        LexicalAnalyzer &lexicalAnalyzer;

    public:
        using RuleBlockInterface::TextIterator;

        [[nodiscard]] RuleReference(
            LexicalAnalyzer &lexical_analyzer, isl::string_view sequence_starter,
            isl::string_view sequence_ender, TextIterator &rule_iterator);

        [[nodiscard]] RuleReference(
            LexicalAnalyzer &lexical_analyzer, isl::string_view sequence_begin_and_end,
            TextIterator &rule_iterator);

        [[nodiscard]] RuleReference(LexicalAnalyzer &lexical_analyzer, std::string rule_name);

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            // at the moment of initialization we may not have this information
            // so it is better to return false to be on the safe side
            return false;
        }

        [[nodiscard]] auto
            scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult override;
    };
}// namespace ccl::lexer::rule

#endif /* CCL_PROJECT_RULE_REFERENCE_HPP */
