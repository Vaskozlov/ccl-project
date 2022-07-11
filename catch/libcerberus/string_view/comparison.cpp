#include <cerberus/debug/debug.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::debug;
using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

CONSTEXPR_TEST
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    assertTrue(cerb_string == std_string);
    return {};
}
();

CONSTEXPR_TEST
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "hello, World!"_sv;

    assertTrue(cerb_string > std_string);
    assertTrue(cerb_string >= std_string);
    return {};
}
();

CONSTEXPR_TEST
{
    auto std_string = "hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    assertTrue(cerb_string < std_string);
    assertTrue(cerb_string <= std_string);
    return {};
}
();


CONSTEXPR_TEST
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "hello, World!"_sv;

    assertTrue(cerb_string > std_string);
    assertTrue(cerb_string >= std_string);
    return {};
}
();

CONSTEXPR_TEST
{
    auto std_string = "Hello, World"sv;
    auto cerb_string = "Hello, World!"_sv;

    assertTrue(cerb_string > std_string);
    assertTrue(cerb_string >= std_string);
    return {};
}
();
