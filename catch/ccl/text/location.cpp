#include <ccl/debug/debug_file.hpp>
#include <ccl/text/location.hpp>

using namespace ccl;
using namespace text;

BOOST_AUTO_TEST_CASE(LocationTracking)
{
    auto location = Location{ "none" };

    BOOST_ASSERT(location.getLine() == 1U);
    BOOST_ASSERT(location.getColumn() == 0U);
    BOOST_ASSERT(location.getRealColumn() == 0U);
    BOOST_ASSERT(location.getFilename() == "none");

    location.intermediateNext('a');
    location.next(U'a');

    BOOST_ASSERT(location.getLine() == 1U);
    BOOST_ASSERT(location.getColumn() == 1U);
    BOOST_ASSERT(location.getRealColumn() == 1U);

    location.intermediateNext('\n');
    location.next(U'\n');

    BOOST_ASSERT(location.getLine() == 2U);
    BOOST_ASSERT(location.getColumn() == 0U);
    BOOST_ASSERT(location.getRealColumn() == 0U);

    location.intermediateNext(static_cast<char>(0b1100'0010));// NOLINT utf8 2 byte character

    BOOST_ASSERT(location.getLine() == 2U);
    BOOST_ASSERT(location.getColumn() == 0U);
    BOOST_ASSERT(location.getRealColumn() == 1U);

    location.intermediateNext(static_cast<char>(0b1000'0000));// NOLINT utf8 2 byte character end
    location.next(U'a');                                      // just some character

    BOOST_ASSERT(location.getLine() == 2U);
    BOOST_ASSERT(location.getColumn() == 1U);
    BOOST_ASSERT(location.getRealColumn() == 2U);
}
