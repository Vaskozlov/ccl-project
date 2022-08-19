#include <boost/test/unit_test.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;
using namespace cerb::text;

BOOST_AUTO_TEST_CASE(LocationFormatting)
{
    auto location = Location{ u8"builtin" };
    BOOST_ASSERT(format<u8"{}">(location) == u8"builtin, line: 1, column: 0");
}
