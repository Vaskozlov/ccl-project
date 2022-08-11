#include <cerberus/const_string.hpp>
#include <cerberus/debug/debug_file.hpp>

using namespace cerb::string_view_literals;

template<cerb::ConstString String>
constexpr static auto testConstString(auto input) -> bool
{
    auto test_string = input;

    assertEqual(String.empty(), test_string.empty());
    assertEqual(String.size(), test_string.size());
    assertEqual(String.strView(), test_string);

    assertTrue(std::ranges::equal(String, test_string));
    assertTrue(std::equal(String.begin(), String.end(), test_string.begin()));

    return true;
}

static_assert(testConstString<u8"">(u8""_sv));
static_assert(testConstString<u8"Hello, World!">(u8"Hello, World!"_sv));
