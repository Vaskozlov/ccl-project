#include <boost/test/unit_test.hpp>
#include <cerberus/format/format.hpp>

using namespace cerb::fmt;

BOOST_AUTO_TEST_CASE(ZeroArgumentsFormatting)
{
    BOOST_ASSERT(format<u8"">().empty());
    BOOST_ASSERT(format<u8"Hello, World!">() == u8"Hello, World!");
}
