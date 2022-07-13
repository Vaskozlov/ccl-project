#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace cerb::debug;

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<"">(), "");
    assertEqual(format<"Hello, World!">(), "Hello, World!");
    return {};
}
();
