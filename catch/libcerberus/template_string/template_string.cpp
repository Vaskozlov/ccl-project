#include <cerberus/debug/debug.hpp>
#include <cerberus/template_string.hpp>

using namespace std::string_view_literals;

template<cerb::TemplateString String>
constexpr auto testTemplateString(std::string_view input) -> bool
{
    auto test_string = input;

    ASSERT_EQ(String.empty(), test_string.empty());
    ASSERT_EQ(String.size(), test_string.size());
    ASSERT_EQ(String.strView(), test_string);

    ASSERT_TRUE(std::ranges::equal(String, test_string));
    ASSERT_TRUE(std::equal(String.begin(), String.end(), test_string.begin()));

    return true;
}

static_assert(testTemplateString<"">(""));
static_assert(testTemplateString<"Hello, World!">("Hello, World!"));
