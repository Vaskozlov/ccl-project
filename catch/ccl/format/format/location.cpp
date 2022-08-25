#include <ccl/debug/debug_file.hpp>
#include <ccl/format/format.hpp>

using namespace ccl::fmt;
using namespace ccl::text;

BOOST_AUTO_TEST_CASE(LocationFormatting)
{
    DEBUG_VAR location = Location{ u8"builtin" };
    BOOST_ASSERT(format<u8"{}">(location) == u8"builtin, line: 1, column: 0");
}
