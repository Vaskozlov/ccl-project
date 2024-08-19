#include <ccl/lexer/rule/rule_reference.hpp>

namespace ccl::lexer::rule
{
    RuleReference::RuleReference(
        LexicalAnalyzer &lexical_analyzer, isl::string_view sequence_starter,
        isl::string_view sequence_ender, RuleBlockInterface::TextIterator &rule_iterator)
      : Sequence{SequenceFlags{.multiline = false, .noEscaping = false,}, sequence_starter, sequence_ender, rule_iterator,}
      , lexicalAnalyzer{lexical_analyzer}
    {}

    RuleReference::RuleReference(
        LexicalAnalyzer &lexical_analyzer, isl::string_view sequence_begin_and_end,
        RuleBlockInterface::TextIterator &rule_iterator)
      : RuleReference{
            lexical_analyzer,
            sequence_begin_and_end,
            sequence_begin_and_end,
            rule_iterator,
        }
    {}

    RuleReference::RuleReference(LexicalAnalyzer &lexical_analyzer, std::string rule_name)
      : Sequence{std::move(rule_name)}
      , lexicalAnalyzer{lexical_analyzer}
    {}

    auto RuleReference::scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult
    {
        const auto *container = lexicalAnalyzer.getByRuleName(getValue());
        return container->scan(text_iterator);
    }
}// namespace ccl::lexer::rule