#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/lex/tokenizer.hpp>

namespace ccl::lex
{
    using namespace std::string_literals;

    LexicalAnalyzer::LexicalAnalyzer(
        ExceptionHandler &exception_handler, InitializerList<Rule> rules, string_view filename,
        std::basic_string<size_t> ignored_ids)
      : ignoredIds{std::move(ignored_ids)}
      , exceptionHandler{exception_handler}
    {
        for (const auto &rule : rules) {
            createContainer(rule.repr, rule.id, filename);
        }
    }

    auto LexicalAnalyzer::createContainer(string_view rule, Id id, string_view filename) -> void
    {
        auto container =
            Container(TextIterator{rule, exceptionHandler, filename}, specialItems, id, true);

        if (container.isSpecial()) {
            specialItems.special_items.emplace_back(std::move(container));
        } else if (!container.empty()) {
            items.emplace_back(std::move(container));
        }
    }

    auto LexicalAnalyzer::getTokenizer(string_view text, string_view filename) -> Tokenizer
    {
        return {*this, text, filename};
    }

    auto LexicalAnalyzer::getTokenizer(
        string_view text, string_view filename, ExceptionHandler &handler) -> Tokenizer
    {
        return {*this, text, filename, handler};
    }
}// namespace ccl::lex
