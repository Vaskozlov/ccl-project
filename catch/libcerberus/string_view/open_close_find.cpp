#include <boost/test/unit_test.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;

BOOST_AUTO_TEST_CASE(StringViewOpenCloseFind)
{
    auto cerb_string = "(Hello(, )World)!"_sv;
    BOOST_ASSERT(cerb_string.openCloseFind('(', ')').value() == cerb_string.rfind(')'));
}
