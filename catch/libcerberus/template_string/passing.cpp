#include <cerberus/debug/debug.hpp>
#include <cerberus/template_string.hpp>

using namespace std::string_view_literals;

// NOLINTBEGIN

template<cerb::TemplateString String>
constexpr auto templateStringTest() -> void
{
    static constexpr std::string_view InputString = "Hello, World!"sv;

    static_assert(!String.empty());

    STATIC_ASSERT_EQ(String.size(), 13UL);
    ASSERT_EQ(String.strView(), InputString);

    ASSERT_TRUE(std::ranges::equal(String, InputString));
    ASSERT_TRUE(std::equal(String.begin(), String.end(), InputString.begin()));
}

DEBUG_SCOPE
{
    templateStringTest<"Hello, World!">();
    return {};
}

// NOLINTEND