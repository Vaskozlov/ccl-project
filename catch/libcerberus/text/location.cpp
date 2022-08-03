#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/location.hpp>

using Location = cerb::text::Location<char>;

CONSTEXPR_TEST
{
    Location location{ "none" };

    assertEqual(location.getLine(), 1U);
    assertEqual(location.getColumn(), 0U);
    assertEqual(location.getFilename(), "none");

    location.next('a');

    assertEqual(location.getLine(), 1U);
    assertEqual(location.getColumn(), 1U);

    location.next('\n');

    assertEqual(location.getLine(), 2U);
    assertEqual(location.getColumn(), 1U);

    return {};
}
();
