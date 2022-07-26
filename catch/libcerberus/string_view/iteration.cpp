#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

CONSTEXPR_TEST
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    assertTrue(std::ranges::equal(std_string, cerb_string));
    assertTrue(std::equal(std_string.begin(), std_string.end(), cerb_string.begin()));

    return {};
}
();
