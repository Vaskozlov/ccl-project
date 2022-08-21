#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(StringViewIteration)
{
    DEBUG_DECL std_string = "Hello, World!"sv;
    DEBUG_DECL cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(std::ranges::equal(std_string, cerb_string));
    BOOST_ASSERT(std::equal(std_string.begin(), std_string.end(), cerb_string.begin()));
}
