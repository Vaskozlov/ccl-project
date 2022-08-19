#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(StringViewFind)
{
    DEBUG_DECL std_string = "Hello, World!"sv;
    DEBUG_DECL cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string.find('H').value() == std_string.find('H'));
    BOOST_ASSERT(cerb_string.find(',').value() == std_string.find(','));
    BOOST_ASSERT(cerb_string.find('!').value() == std_string.find('!'));

    BOOST_ASSERT(not cerb_string.find('A').has_value());

    BOOST_ASSERT(cerb_string.find('l', 6).value() == std_string.find('l', 6));
    BOOST_ASSERT(cerb_string.find('l', cerb_string.begin() + 6).value() == std_string.find('l', 6));
}
