#include <ccl/debug/debug_file.hpp>
#include <ccl/format/format.hpp>

using namespace ccl::fmt;
using namespace std::string_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(StringFormatting)
{
    BOOST_ASSERT(format<u8"{}">(u8"Hello, "s) == u8"Hello, ");
    BOOST_ASSERT(format<u8"{}">(u8"World!"s) == u8"World!");
    BOOST_ASSERT(format<u8"{}, {}!">(u8"Hello"s, u8"World") == u8"Hello, World!");
}
