#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

auto convert(cerb::u8string_view str) -> std::string_view
{
    return { reinterpret_cast<const char *>(str.begin()), str.size() };// NOLINT
}

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text = TextIterator{ u8"привет фф 1986" };
    auto analyzer = LexicalAnalyzer{ { 1, u8R"([а-я]+)" }, { 2, u8"[0-9]+" } };
    auto tokens = analyzer.getTokenizer(text);

    while (tokens()) {
        auto &token = tokens.getValue();
        fmt::print("{}: {}\n", token.getId(), convert(token.getRepr()));
    }

    return {};
}
();
