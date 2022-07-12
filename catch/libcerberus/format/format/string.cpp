#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace cerb::debug;
using namespace std::string_literals;
using namespace std::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<"{}">("Hello, "s), "Hello, "s);
    assertEqual(format<"{}">("World!"s), "World!"s);
    assertEqual(format<"{}, {}!">("Hello"s, "World"s), "Hello, World!"s);
    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<"{}">("Hello, "sv), "Hello, "s);
    assertEqual(format<"{}">("World!"sv), "World!"s);
    assertEqual(format<"{}, {}!">("Hello"sv, "World"sv), "Hello, World!"s);
    return {};
}
();
