#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace string_view_literals;

TEST_CASE("StringViewOpenCloseFind", "[StringView]")
{
    DEBUG_VAR own_string = "(Hello(, )World)!"_sv;
    REQUIRE(own_string.findMatchingPair('(', ')').value() == own_string.rfind(')'));
}
