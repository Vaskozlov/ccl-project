#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace cerb::debug;
using namespace std::string_literals;

// NOLINTBEGIN

STRING_TEST
{
    assertEqual(format<"{}">(0), "0"s);
    assertEqual(format<"{}">(10), "10"s);
    return {};
}
();

STRING_TEST
{
    assertEqual(format<"Hello, {} World!">(-10), "Hello, -10 World!"s);
    assertEqual(format<"Hello, {} World! {}">(-10, 100), "Hello, -10 World! 100"s);
    return {};
}
();

// NOLINTEND
