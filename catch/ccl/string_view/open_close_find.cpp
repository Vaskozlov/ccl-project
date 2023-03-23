#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace string_view_literals;

TEST_CASE("StringViewOpenCloseFind", "[StringView]")
{
    DEBUG_VAR cerb_string = "(Hello(, )World)!"_sv;
    REQUIRE(cerb_string.openCloseFind('(', ')').value() == cerb_string.rfind(')'));
}
