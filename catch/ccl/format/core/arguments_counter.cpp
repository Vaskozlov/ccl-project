#include <ccl/debug/debug_file.hpp>
#include <ccl/format/core/arguments_counter.hpp>

using namespace ccl::fmt::core;
using namespace ccl::string_view_literals;

BOOST_AUTO_TEST_CASE(FormatCountArguments)
{
    BOOST_CHECK_EQUAL(ArgumentsCounter{ u8"" }.get(), 0);
    BOOST_CHECK_EQUAL(ArgumentsCounter{ u8"Hello!{}" }.get(), 1);
    BOOST_CHECK_EQUAL(ArgumentsCounter{ u8"Hello {}, { }!" }.get(), 2);
    BOOST_CHECK_EQUAL(ArgumentsCounter{ u8"Hello {}{{, {}}}!_" }.get(), 2);
}
