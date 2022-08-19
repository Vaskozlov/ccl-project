#include <boost/test/unit_test.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(StringViewRfind)
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string.rfind('H').value() == std_string.rfind('H'));
    BOOST_ASSERT(cerb_string.rfind(',').value() == std_string.rfind(','));
    BOOST_ASSERT(cerb_string.rfind('!').value() == std_string.rfind('!'));
    BOOST_ASSERT(not cerb_string.rfind('A').has_value());
}
