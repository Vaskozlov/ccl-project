#include <cerberus/debug/debug_file.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<"">(), "");
    assertEqual(format<"Hello, World!">(), "Hello, World!");
    return {};
}
();
