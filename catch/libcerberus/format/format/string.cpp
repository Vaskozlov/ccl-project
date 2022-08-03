#include <cerberus/debug/debug_file.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace std::string_literals;
using namespace std::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<"{}">("Hello, "s), "Hello, ");
    assertEqual(format<"{}">("World!"s), "World!");
    assertEqual(format<"{}, {}!">("Hello"s, "World"), "Hello, World!");
    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<"{}">("Hello, "sv), "Hello, ");
    assertEqual(format<"{}">("World!"sv), "World!");
    assertEqual(format<"{}, {}!">("Hello"sv, "World"), "Hello, World!");
    return {};
}
();
