#include <ccl/lex/lexical_analyzer.hpp>

namespace ccl::lex
{
    using namespace std::string_literals;

    LexicalAnalyzer::LexicalAnalyzer(
        const std::initializer_list<std::pair<size_t, u8string_view>> &rules_,
        u8string_view filename, const CommentTokens &comment_tokens_)
    {
        for (const auto &[id, rule] : rules_) {
            createContainer(rule, id, comment_tokens_, filename);
        }

        const auto &errors_in_rules = exception_accumulator.getErrors();

        if (not errors_in_rules.empty()) {
            throw UnrecoverableError{
                "unable to create lexical analyzer, while there are errors in rules"
            };
        }
    }

    auto LexicalAnalyzer::createContainer(
        u8string_view rule, size_t id, const CommentTokens &comment_tokens, u8string_view filename)
        -> void
    {
        auto container = Container(
            TextIterator{ rule, &exception_accumulator, comment_tokens, filename }, id, shared);

        if (not container.empty()) {
            items.emplace(std::move(container));
        }
    }

}// namespace ccl::lex
