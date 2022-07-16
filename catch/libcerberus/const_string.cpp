#include <cerberus/const_string.hpp>
#include <cerberus/debug/debug.hpp>

using namespace cerb::debug;
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

static_assert(testConstString<"">(""_sv));
static_assert(testConstString<"Hello, World!">("Hello, World!"_sv));

static_assert(testConstString<u"">(u""_sv));
static_assert(testConstString<u"Hello, World!">(u"Hello, World!"_sv));
