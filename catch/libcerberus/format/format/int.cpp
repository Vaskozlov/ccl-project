#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>


using namespace std::string_literals;

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STRING_ASSERT_EQ(cerb::fmt::format<"{}">(0), "0"s);
    STRING_ASSERT_EQ(cerb::fmt::format<"{}">(10), "10"s);
    return {};
}

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STRING_ASSERT_EQ(cerb::fmt::format<"Hello, {} World!">(-10), "Hello, -10 World!"s);
    STRING_ASSERT_EQ(cerb::fmt::format<"Hello, {} World! {}">(-10, 100), "Hello, -10 World! 100"s);
    return {};
}
