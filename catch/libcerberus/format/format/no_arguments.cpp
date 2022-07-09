#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace std::string_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    ASSERT_EQ(format<"">(), ""s);
    ASSERT_EQ(format<"Hello, World!">(), "Hello, World!"s);
    return {};
}
();
