#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/peg_parser.hpp>

namespace ccl::lexer
{
    LexicalAnalyzer::LexicalAnalyzer(ExceptionHandler &exception_handler)
      : exceptionHandler{exception_handler}
    {}

    LexicalAnalyzer::LexicalAnalyzer(
        ExceptionHandler &exception_handler, const std::initializer_list<Rule> &rules,
        isl::string_view filename, std::vector<std::size_t> ignored_ids)
      : ignoredIds{std::move(ignored_ids)}
      , exceptionHandler{exception_handler}
    {
        for (const auto &rule : rules) {
            createContainer(rule, rule.id, filename);
        }
    }

    auto
        LexicalAnalyzer::createContainer(const Rule &rule, Id id, isl::string_view filename) -> void
    {
        auto container = isl::makeUnique<Container>(
            *this, TextIterator{rule.repr, exceptionHandler, filename}, id, true);

        addContainer(rule.name, std::move(container));
    }

    auto
        LexicalAnalyzer::getTokenizer(isl::string_view text, isl::string_view filename) -> Tokenizer
    {
        return {*this, text, filename};
    }

    auto LexicalAnalyzer::getTokenizer(
        isl::string_view text, isl::string_view filename, ExceptionHandler &handler) -> Tokenizer
    {
        return {*this, text, filename, handler};
    }

    [[nodiscard]] auto LexicalAnalyzer::getParser(
        isl::string_view rule_name, isl::string_view text, isl::string_view filename,
        ExceptionHandler &handler) -> PegParser
    {
        return {rule_name, *this, text, filename, handler};
    }

    [[nodiscard]] auto LexicalAnalyzer::getParser(
        isl::string_view rule_name, isl::string_view text, isl::string_view filename) -> PegParser
    {
        return getParser(rule_name, text, filename, exceptionHandler);
    }
}// namespace ccl::lexer
