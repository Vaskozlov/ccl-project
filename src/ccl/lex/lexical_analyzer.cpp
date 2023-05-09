#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/lex/tokenizer.hpp>

namespace ccl::lex
{
    using namespace std::string_literals;

    LexicalAnalyzer::LexicalAnalyzer(
        ExceptionHandler &exception_handler, InitializerList<Rule> rules, std::string_view filename,
        Vector<size_t> ignored_ids)
      : ignoredIds{std::move(ignored_ids)}
      , exceptionHandler{exception_handler}
    {
        for (const Rule &rule : rules) {
            createContainer(rule.repr, rule.id, filename);
        }
    }

    auto LexicalAnalyzer::createContainer(string_view rule, Id id, std::string_view filename)
        -> void
    {
        auto container =
            Container(TextIterator{rule, exceptionHandler, filename}, specialItems, id, true);

        if (container.isSpecial()) {
            specialItems.specialItems.emplace_back(std::move(container));
        } else if (!container.empty()) {
            items.emplace_back(std::move(container));
        }
    }

    auto LexicalAnalyzer::getTokenizer(string_view text, std::string_view filename) -> Tokenizer
    {
        return {*this, text, filename};
    }

    auto LexicalAnalyzer::getTokenizer(
        string_view text, std::string_view filename, ExceptionHandler &handler) -> Tokenizer
    {
        return {*this, text, filename, handler};
    }
}// namespace ccl::lex
