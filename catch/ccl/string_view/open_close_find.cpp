#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace string_view_literals;

BOOST_AUTO_TEST_CASE(StringViewOpenCloseFind)
{
    DEBUG_VAR cerb_string = "(Hello(, )World)!"_sv;
    BOOST_ASSERT(cerb_string.openCloseFind('(', ')').value() == cerb_string.rfind(')'));
}
