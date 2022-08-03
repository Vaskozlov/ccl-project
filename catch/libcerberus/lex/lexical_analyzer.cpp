#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text = TextIterator<char>{ "test 123" };
    auto analyzer = LexicalAnalyzer<char>{ { 0, R"("test")" } };

    analyzer.yield(text);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text = TextIterator<char>{ "abcd 131" };
    auto analyzer = LexicalAnalyzer<char>{ { 0, R"([a-z]+[0-9]*)" } };

    analyzer.yield(text);

    return {};
}
();
