#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/location.hpp>

using Location = cerb::text::Location;

CONSTEXPR_TEST
{
    Location location{ u8"none" };

    assertEqual(location.getLine(), 1U);
    assertEqual(location.getColumn(), 0U);
    assertEqual(location.getRealColumn(), 0U);
    assertEqual(location.getFilename(), u8"none");

    location.intermediateNext(u8'a');
    location.next(U'a');

    assertEqual(location.getLine(), 1U);
    assertEqual(location.getColumn(), 1U);
    assertEqual(location.getRealColumn(), 1U);

    location.intermediateNext(u8'\n');
    location.next(U'\n');

    assertEqual(location.getLine(), 2U);
    assertEqual(location.getColumn(), 0U);
    assertEqual(location.getRealColumn(), 0U);

    location.intermediateNext(0b1100'0010);// NOLINT utf8 2 byte character

    assertEqual(location.getLine(), 2U);
    assertEqual(location.getColumn(), 0U);
    assertEqual(location.getRealColumn(), 1U);

    location.intermediateNext(0b1000'0000);// NOLINT utf8 2 byte character end
    location.next(U'a');                   // just some character

    assertEqual(location.getLine(), 2U);
    assertEqual(location.getColumn(), 1U);
    assertEqual(location.getRealColumn(), 2U);

    return {};
}
();
