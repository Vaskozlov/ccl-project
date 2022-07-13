#include <cerberus/debug/debug.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::debug;
using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

CONSTEXPR_TEST
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    // NOLINTBEGIN

    assertTrue(cerb_string.substr(0, 3) == std_string.substr(0, 3));
    assertTrue(cerb_string.substr(3, 3) == std_string.substr(3, 3));
    assertTrue(cerb_string.substr(3, 20) == std_string.substr(3, 20));
    assertTrue(cerb_string.substr(10, 20) == std_string.substr(10, 20));

    // NOLINTEND

    return {};
}
();
