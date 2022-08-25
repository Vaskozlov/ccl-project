#include <ccl/debug/debug_file.hpp>
#include <ccl/format/format.hpp>

using namespace ccl::fmt;

BOOST_AUTO_TEST_SUITE(IntFormatting)

BOOST_AUTO_TEST_CASE(OnlyIntFormatting)
{
    BOOST_ASSERT(format<u8"{}">(0) == u8"0");
    BOOST_ASSERT(format<u8"{}">(10) == u8"10");
}

BOOST_AUTO_TEST_CASE(IntFormattingWithString)
{
    BOOST_ASSERT(format<u8"Hello, {} World!">(-10) == u8"Hello, -10 World!");
    BOOST_ASSERT(format<u8"Hello, {} World! {}">(-10, 100) == u8"Hello, -10 World! 100");
}

BOOST_AUTO_TEST_SUITE_END()
