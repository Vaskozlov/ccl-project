#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(StringViewSubstr)
{
    DEBUG_DECL std_string = "Hello, World!"sv;
    DEBUG_DECL cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string.substr(0, 3) == std_string.substr(0, 3));
    BOOST_ASSERT(cerb_string.substr(3, 3) == std_string.substr(3, 3));
    BOOST_ASSERT(cerb_string.substr(3, 20) == std_string.substr(3, 20));
    BOOST_ASSERT(cerb_string.substr(10, 20) == std_string.substr(10, 20));
}
