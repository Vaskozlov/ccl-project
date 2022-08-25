#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::string_view_literals;

static auto convert(cerb::u8string_view str) -> std::string_view
{
    return { reinterpret_cast<const char *>(str.begin()), str.size() };// NOLINT
}

BOOST_AUTO_TEST_CASE(LexTest)
{
    auto text =
        u8R"(
        auto i = 10;
        i + 10; // test
        auto _ = "Hello, World!" + "test")"_sv;


    // NOLINTBEGIN
    auto analyzer =
        LexicalAnalyzer{ { 1, u8R"([a-zA-Z_]+)" }, { 2, u8"[0-9]+" },   { 3, u8R"( '+')" },
                         { 4, u8R"(';')" },        { 5, u8R"('=')" },   { 6, u8R"("\""s)" },
                         { 7, u8R"('\n')" },       { 0, u8R"("//"co)" } };
    auto tokenizer = analyzer.getTokenizer(text);
    // NOLINTEND

    auto token = tokenizer.yield();

    while (token) {
        fmt::print("{}: {}\n", token.getId(), convert(token.getRepr()));
        token = tokenizer.yield();
    }
}
