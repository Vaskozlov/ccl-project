#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace cerb::text;
using namespace cerb::debug;
using namespace std::string_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto location = Location<char>{ "builtin" };
    assertEqual(format<"{}">(location), "filename: builtin, line: 1, column: 0"s);
    return {};
}
();
