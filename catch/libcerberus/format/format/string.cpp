#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STRING_ASSERT_EQ(cerb::fmt::format<"{}">("Hello, "s), "Hello, "s);
    STRING_ASSERT_EQ(cerb::fmt::format<"{}">("World!"s), "World!"s);
    STRING_ASSERT_EQ(cerb::fmt::format<"{}, {}!">("Hello"s, "World"s), "Hello, World!"s);
    return {};
}

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STRING_ASSERT_EQ(cerb::fmt::format<"{}">("Hello, "sv), "Hello, "s);
    STRING_ASSERT_EQ(cerb::fmt::format<"{}">("World!"sv), "World!"s);
    STRING_ASSERT_EQ(cerb::fmt::format<"{}, {}!">("Hello"sv, "World"sv), "Hello, World!"s);
    return {};
}
