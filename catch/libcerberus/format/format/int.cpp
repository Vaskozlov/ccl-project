#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace cerb::debug;

// NOLINTBEGIN

STRING_TEST
{
    assertEqual(format<"{}">(0), "0");
    assertEqual(format<"{}">(10), "10");
    return {};
}
();

STRING_TEST
{
    assertEqual(format<char, u"Hello, {} World!">(-10), "Hello, -10 World!");
    assertEqual(format<"Hello, {} World! {}">(-10, 100), "Hello, -10 World! 100");
    return {};
}
();

STRING_TEST
{
    assertEqual(format<char16_t, "Hello, {} World!">(-10), u"Hello, -10 World!");
    assertEqual(format<u"Hello, {} World! {}">(-10, 100), u"Hello, -10 World! 100");
    return {};
}
();

// NOLINTEND
