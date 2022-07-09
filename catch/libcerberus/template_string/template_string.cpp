#include <cerberus/debug/debug.hpp>
#include <cerberus/template_string.hpp>

using namespace std::string_view_literals;

CONSTEXPR_TEST
{
    auto test_string = ""sv;
    auto template_string = cerb::TemplateString{ "" };

    ASSERT_EQ(template_string.empty(), test_string.empty());
    ASSERT_EQ(template_string.size(), test_string.size());
    ASSERT_EQ(template_string.strView(), test_string);

    ASSERT_TRUE(std::ranges::equal(template_string, test_string));
    ASSERT_TRUE(std::equal(template_string.begin(), template_string.end(), test_string.begin()));

    return {};
}
();

CONSTEXPR_TEST
{
    auto test_string = "Hello, World!"sv;
    auto template_string = cerb::TemplateString{ "Hello, World!" };

    ASSERT_EQ(template_string.empty(), test_string.empty());
    ASSERT_EQ(template_string.size(), test_string.size());
    ASSERT_EQ(template_string.strView(), test_string);

    ASSERT_TRUE(std::ranges::equal(template_string, test_string));
    ASSERT_TRUE(std::equal(template_string.begin(), template_string.end(), test_string.begin()));

    return {};
}
();
