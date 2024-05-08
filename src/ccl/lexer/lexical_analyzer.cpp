#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/tokenizer.hpp>

namespace ccl::lexer
{
    LexicalAnalyzer::LexicalAnalyzer(
        ExceptionHandler &exception_handler, const std::initializer_list<Rule> &rules,
        std::string_view filename, std::vector<size_t> ignored_ids)
      : ignoredIds{std::move(ignored_ids)}
      , exceptionHandler{exception_handler}
    {
        for (const Rule &rule : rules) {
            createContainer(rule.repr, rule.id, filename);
        }
    }

    auto LexicalAnalyzer::createContainer(isl::string_view rule, Id id, std::string_view filename)
        -> void
    {
        auto container =
            Container(TextIterator{rule, exceptionHandler, filename}, anyPlaceItems, id, true);

        if (container.isAnyPlaceRule()) {
            anyPlaceItems.items.emplace_back(std::move(container));
        } else if (!container.empty()) {
            items.emplace_back(std::move(container));
        }
    }

    auto LexicalAnalyzer::getTokenizer(isl::string_view text, std::string_view filename)
        -> Tokenizer
    {
        return {*this, text, filename};
    }

    auto LexicalAnalyzer::getTokenizer(
        isl::string_view text, std::string_view filename, ExceptionHandler &handler) -> Tokenizer
    {
        return {*this, text, filename, handler};
    }
}// namespace ccl::lexer
