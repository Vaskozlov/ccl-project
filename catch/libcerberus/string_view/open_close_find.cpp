#include <cerberus/debug/debug.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::debug;
using namespace cerb::string_view_literals;

CONSTEXPR_TEST
{
    auto cerb_string = "(Hello(, )World)!"_sv;
    assertEqual(cerb_string.openCloseFind('(', ')'), cerb_string.rfind(')'));
    return {};
}
();
