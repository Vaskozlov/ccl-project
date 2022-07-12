#include <cerberus/debug/debug.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace cerb::debug;
using namespace std::string_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    assertEqual(format<"">(), ""s);
    assertEqual(format<"Hello, World!">(), "Hello, World!"s);
    return {};
}
();
