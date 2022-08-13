#include <cerberus/debug/debug_file.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace cerb::text;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto location = Location{ u8"builtin" };
    assertEqual(format<u8"{}">(location), u8"builtin, line: 1, column: 0");
    return {};
}
();
