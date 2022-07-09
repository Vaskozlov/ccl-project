#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace std::string_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    ASSERT_EQ(format<"{}">(0), "0"s);
    ASSERT_EQ(format<"{}">(10), "10"s);
    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    ASSERT_EQ(format<"Hello, {} World!">(-10), "Hello, -10 World!"s);
    ASSERT_EQ(format<"Hello, {} World! {}">(-10, 100), "Hello, -10 World! 100"s);
    return {};
}
();
