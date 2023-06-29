#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace string_view_literals;
using namespace std::string_view_literals;

TEST_CASE("StringViewWithoutLastCharacter", "[StringView]")
{
    REQUIRE("Hello, World!"_sv.removeLastCharacter() == "Hello, World"_sv);
    REQUIRE(""_sv.removeLastCharacter() == ""_sv);
}
