#include <boost/test/unit_test.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(StringViewIteration)
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(std::ranges::equal(std_string, cerb_string));
    BOOST_ASSERT(std::equal(std_string.begin(), std_string.end(), cerb_string.begin()));
}
