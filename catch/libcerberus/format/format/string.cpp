#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace std::string_literals;
using namespace std::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    ASSERT_EQ(format<"{}">("Hello, "s), "Hello, "s);
    ASSERT_EQ(format<"{}">("World!"s), "World!"s);
    ASSERT_EQ(format<"{}, {}!">("Hello"s, "World"s), "Hello, World!"s);
    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    ASSERT_EQ(format<"{}">("Hello, "sv), "Hello, "s);
    ASSERT_EQ(format<"{}">("World!"sv), "World!"s);
    ASSERT_EQ(format<"{}, {}!">("Hello"sv, "World"sv), "Hello, World!"s);
    return {};
}
();
