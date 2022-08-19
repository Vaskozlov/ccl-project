#include <boost/test/unit_test.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

auto convert(cerb::u8string_view str) -> std::string_view
{
    return { reinterpret_cast<const char *>(str.begin()), str.size() };  // NOLINT
}

BOOST_AUTO_TEST_CASE(LexTest)
{
    auto text = TextIterator{ u8R"(auto i = 10; i + 10; auto _ = "Hello, World!" +)" };

    // NOLINTBEGIN
    auto analyzer =
        LexicalAnalyzer{ { 1, u8R"([a-zA-Z_]+)" }, { 2, u8"[0-9]+" }, { 3, u8R"( '+')" },
                         { 4, u8R"(';')" },        { 5, u8R"('=')" }, { 6, u8R"("\""s)" } };
    // NOLINTEND

    auto token = analyzer.yield(text);

    while (token) {
        fmt::print("{}: {}\n", token.getId(), convert(token.getValue()));
        token = analyzer.yield(text);
    }
}
