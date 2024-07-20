#include <ccl/debug/debug.hpp>
#include <ccl/text/remove_escaping.hpp>

TEST_CASE("RemoveEscaping", "[Escaping]")
{
    using namespace ccl;
    using namespace isl::string_view_literals;

    auto input = R"(Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF)"_sv;
    auto expected = "Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto conversion_result = text::removeEscaping(input, {});

    REQUIRE(conversion_result == expected);
}
