#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

// NOLINTBEGIN

CONSTEXPR_TEST
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    assertEqual(cerb_string.find('H').value(), std_string.find('H'));
    assertEqual(cerb_string.find(',').value(), std_string.find(','));
    assertEqual(cerb_string.find('!').value(), std_string.find('!'));

    assertFalse(cerb_string.find('A').has_value());

    assertEqual(cerb_string.find('l', 6).value(), std_string.find('l', 6));
    assertEqual(cerb_string.find('l', cerb_string.begin() + 6).value(), std_string.find('l', 6));

    return {};
}
();

// NOLINTEND
