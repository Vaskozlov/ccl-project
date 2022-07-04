#include <cerberus/debug/debug.hpp>
#include <cerberus/template_string.hpp>

using namespace std::string_view_literals;

// NOLINTBEGIN

template<cerb::TemplateString String>
constexpr auto templateStringTest(std::string_view input_string) -> void
{
    ASSERT_EQ(String.empty(), input_string.empty());
    ASSERT_EQ(String.size(), input_string.size());
    ASSERT_EQ(String.strView(), input_string);

    ASSERT_TRUE(std::ranges::equal(String, input_string));
    ASSERT_TRUE(std::equal(String.begin(), String.end(), input_string.begin()));
}

DEBUG_SCOPE
{
    templateStringTest<"">("");
    templateStringTest<"Hello, World!">("Hello, World!");
    return {};
}

// NOLINTEND