#include <cerberus/debug/debug_file.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;

// NOLINTBEGIN

STRING_TEST
{
    assertEqual(format<u8"{}">(0), u8"0");
    assertEqual(format<u8"{}">(10), u8"10");
    return {};
}
();

STRING_TEST
{
    assertEqual(format<u8"Hello, {} World!">(-10), u8"Hello, -10 World!");
    assertEqual(format<u8"Hello, {} World! {}">(-10, 100), u8"Hello, -10 World! 100");
    return {};
}
();

// NOLINTEND
