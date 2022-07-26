#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>
#include <string>

using namespace cerb;
using namespace std::string_literals;

// NOLINTBEGIN

CONSTEXPR_TEST
{
    auto string = BasicStringView<char>{ "" };
    assertEqual(string.size(), 0_ZU);
    return {};
}
();

CONSTEXPR_TEST
{
    auto string = BasicStringView<char>{ "Hello, World!" };
    assertEqual(string.size(), 13_ZU);
    return {};
}
();

CONSTEXPR_TEST
{
    const char *str = "";
    auto string = BasicStringView<char>{ str };
    assertEqual(string.size(), 0_ZU);
    return {};
}
();

CONSTEXPR_TEST
{
    const char *str = "Hello, World!";
    auto string = BasicStringView<char>{ str };
    assertEqual(string.size(), 13_ZU);
    return {};
}
();

RUNTIME_TEST
{
    const char *str = "";
    auto string = BasicStringView<char>{ str };
    assertEqual(string.size(), 0_ZU);
    return {};
}
();

RUNTIME_TEST
{
    const char *str = "Hello, World!";
    auto string = BasicStringView<char>{ str };
    assertEqual(string.size(), 13_ZU);
    return {};
}
();

RUNTIME_TEST
{
    auto str = "Hello, World!"s;
    auto string = BasicStringView<char>{ str.data(), str.data() + str.size() };
    assertEqual(string.size(), 13_ZU);
    return {};
}
();

// NOLINTEND
