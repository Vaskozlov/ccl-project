#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator_modules/location.hpp>

using namespace cerb::debug;
using Location = cerb::text::Location<>;

CONSTEXPR_TEST
{
    Location location{ "none" };

    assertEqual(location.getLine(), 1U);
    assertEqual(location.getColumn(), 1U);
    assertEqual(location.getFilename(), "none");

    location.next('a');

    assertEqual(location.getLine(), 1U);
    assertEqual(location.getColumn(), 2U);

    location.next('\n');

    assertEqual(location.getLine(), 2U);
    assertEqual(location.getColumn(), 1U);

    return {};
}
();
