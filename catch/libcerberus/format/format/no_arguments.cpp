#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace std::string_literals;

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STRING_ASSERT_EQ(cerb::fmt::format<"">(), ""s);
    STRING_ASSERT_EQ(cerb::fmt::format<"Hello, World!">(), "Hello, World!"s);
    return {};
}
