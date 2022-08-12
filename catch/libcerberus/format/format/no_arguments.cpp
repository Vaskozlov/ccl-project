#include <cerberus/debug/debug_file.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<u8"">(), u8"");
    assertEqual(format<u8"Hello, World!">(), u8"Hello, World!");
    return {};
}
();
