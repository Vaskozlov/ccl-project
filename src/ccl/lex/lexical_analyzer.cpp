#include <ccl/lex/lexical_analyzer.hpp>

namespace ccl::lex
{
    using namespace std::string_literals;

    LexicalAnalyzer::LexicalAnalyzer(
        ExceptionHandler &exception_handler_,
        const std::initializer_list<std::pair<size_t, string_view>> &rules_, string_view filename,
        const CommentTokens &comment_tokens_)
      : exception_handler(exception_handler_)
    {
        for (const auto &[id, rule] : rules_) {
            createContainer(rule, id, comment_tokens_, filename);
        }
    }

    auto LexicalAnalyzer::createContainer(
        string_view rule, size_t id, const CommentTokens &comment_tokens, string_view filename)
        -> void
    {
        auto container = Container(
            TextIterator{ rule, exception_handler, comment_tokens, filename }, id, shared);

        if (not container.empty()) {
            items.emplace(std::move(container));
        }
    }

}// namespace ccl::lex
