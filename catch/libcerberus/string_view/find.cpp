#include <cerberus/debug/debug.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::debug;
using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

CONSTEXPR_TEST
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    assertEqual(cerb_string.find('H'), std_string.find('H'));
    assertEqual(cerb_string.find(','), std_string.find(','));
    assertEqual(cerb_string.find('!'), std_string.find('!'));
    assertEqual(cerb_string.find('A'), std_string.find('A'));

    return {};
}
();
