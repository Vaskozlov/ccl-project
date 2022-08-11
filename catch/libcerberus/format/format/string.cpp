#include <cerberus/debug/debug_file.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace std::string_literals;
using namespace std::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<u8"{}">("Hello, "s), u8"Hello, ");
    assertEqual(format<u8"{}">("World!"s), u8"World!");
    assertEqual(format<u8"{}, {}!">("Hello"s, "World"), u8"Hello, World!");
    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<u8"{}">(u8"Hello, "sv), u8"Hello, ");
    assertEqual(format<u8"{}">(u8"World!"sv), u8"World!");
    assertEqual(format<u8"{}, {}!">(u8"Hello"sv, "World"), u8"Hello, World!");
    return {};
}
();
