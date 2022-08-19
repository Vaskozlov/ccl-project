#include <cerberus/debug/debug_file.hpp>
#include <cerberus/format/core/arguments_counter.hpp>

using namespace cerb::fmt::core;
using namespace cerb::string_view_literals;

BOOST_AUTO_TEST_CASE(FormatCountArguments)
{
    BOOST_CHECK_EQUAL(ArgumentsCounter{u8""}.get(), 0);
    BOOST_CHECK_EQUAL(ArgumentsCounter{u8"Hello!{}"}.get(), 1);
    BOOST_CHECK_EQUAL(ArgumentsCounter{u8"Hello {}, { }!"}.get(), 2);
    BOOST_CHECK_EQUAL(ArgumentsCounter{u8"Hello {}{{, {}}}!_"}.get(), 2);
}
