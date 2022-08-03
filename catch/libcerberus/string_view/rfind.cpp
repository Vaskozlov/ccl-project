#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

CONSTEXPR_TEST
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    assertEqual(cerb_string.rfind('H'), std_string.rfind('H'));
    assertEqual(cerb_string.rfind(','), std_string.rfind(','));
    assertEqual(cerb_string.rfind('!'), std_string.rfind('!'));
    assertEqual(cerb_string.rfind('A'), std_string.rfind('A'));

    return {};
}
();
