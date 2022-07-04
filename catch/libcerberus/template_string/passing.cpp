#include <cerberus/debug/debug.hpp>
#include <cerberus/template_string.hpp>

using namespace std::string_view_literals;

// NOLINTBEGIN

template<cerb::TemplateString String>
constexpr auto templateStringTest() -> void
{
    static constexpr auto input_string = "Hello, World!"sv;

    static_assert(!String.empty());

    STATIC_ASSERT_EQ(String.size(), 13UL);
    ASSERT_EQ(String.strView(), input_string);

    ASSERT_TRUE(std::ranges::equal(String, input_string));
    ASSERT_TRUE(std::equal(String.begin(), String.end(), input_string.begin()));
}

DEBUG_SCOPE
{
    templateStringTest<"Hello, World!">();
    return {};
}

// NOLINTEND