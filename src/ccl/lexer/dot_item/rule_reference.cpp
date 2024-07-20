#include <ccl/lexer/rule/rule_reference.hpp>

namespace ccl::lexer::rule
{
    RuleReference::RuleReference(
        LexicalAnalyzer &lexical_analyzer, isl::string_view sequence_starter,
        isl::string_view sequence_ender, RuleBlockInterface::TextIterator &rule_iterator,
        Id item_id)
      : Sequence{SequenceFlags{.multiline = false, .noEscaping = false}, sequence_starter, sequence_ender, rule_iterator, item_id}
      , lexicalAnalyzer{lexical_analyzer}
    {}

    RuleReference::RuleReference(
        LexicalAnalyzer &lexical_analyzer, isl::string_view sequence_begin_and_end,
        RuleBlockInterface::TextIterator &rule_iterator, Id item_id)
      : RuleReference{
            lexical_analyzer, sequence_begin_and_end, sequence_begin_and_end, rule_iterator,
            item_id}
    {}

    auto RuleReference::scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult
    {
        auto *container = lexicalAnalyzer.getByRuleName(getValue());
        return container->scan(text_iterator);
    }

    auto RuleReference::parseIteration(const ForkedGenerator &text_iterator) const -> ParsingResult
    {
        auto *container = lexicalAnalyzer.getByRuleName(getValue());
        return container->parse(text_iterator);
    }
}// namespace ccl::lexer::dot_item