#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::lex
{
    using namespace std::string_literals;

    LexicalAnalyzer::LexicalAnalyzer(
        const std::initializer_list<std::pair<size_t, u8string_view>> &rules_,
        u8string_view filename, const CommentTokens &comment_tokens_)
    {
        for (const auto &[id, rule] : rules_) {
            errors += createDotItem(rule, id, comment_tokens_, filename);
        }
    }

    auto LexicalAnalyzer::createDotItem(
        u8string_view rule, size_t id, const CommentTokens &comment_tokens, u8string_view filename)
        -> size_t
    {
        items.emplace_back(
            TextIterator{ rule, &exception_accumulator, comment_tokens, filename }, id, shared);
        return 0;
    }

}// namespace cerb::lex
