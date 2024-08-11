#include <ccl/parser/peg_parser.hpp>

namespace ccl::lexer
{
    LexicalAnalyzer::PegParser::PegParser(
        isl::string_view rule_name, const LexicalAnalyzer &lexical_analyzer, isl::string_view text,
        isl::string_view filename, ExceptionHandler &exception_handler)
      : textIterator{text, exception_handler, filename}
      , mainItem{lexical_analyzer.getByRuleName(rule_name)}
    {}

    LexicalAnalyzer::PegParser::PegParser(
        isl::string_view rule_name, const LexicalAnalyzer &lexical_analyzer, isl::string_view text,
        isl::string_view filename)
      : PegParser{rule_name, lexical_analyzer, text, filename, lexical_analyzer.exceptionHandler}
    {}

    auto LexicalAnalyzer::PegParser::parse() -> isl::UniquePtr<parser::ast::Node>
    {
        return mainItem->parse(textIterator.fork()).getAndReleaseNode();
    }
}// namespace ccl::lexer
