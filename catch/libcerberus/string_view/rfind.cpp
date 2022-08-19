#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(StringViewRfind)
{
    DEBUG_DECL std_string = "Hello, World!"sv;
    DEBUG_DECL cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string.rfind('H').value() == std_string.rfind('H'));
    BOOST_ASSERT(cerb_string.rfind(',').value() == std_string.rfind(','));
    BOOST_ASSERT(cerb_string.rfind('!').value() == std_string.rfind('!'));
    BOOST_ASSERT(not cerb_string.rfind('A').has_value());
}
