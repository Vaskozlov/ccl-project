#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace std::string_literals;

BOOST_AUTO_TEST_CASE(StringViewConstructionEmptyArray)
{
    DEBUG_VAR string = string_view{ "" };
    BOOST_ASSERT(string.empty());
}

BOOST_AUTO_TEST_CASE(StringViewConstructionArray)
{
    DEBUG_VAR string = string_view{ "Hello, World!" };
    BOOST_ASSERT(string.size() == 13);
}

BOOST_AUTO_TEST_CASE(StringViewConstructionEmptyPointer)
{
    const char *str = "";
    DEBUG_VAR string = string_view{ str };
    BOOST_ASSERT(string.empty());
}

BOOST_AUTO_TEST_CASE(StringViewConstructionPointer)
{
    const char *str = "Hello, World!";
    DEBUG_VAR string = string_view{ str };
    BOOST_ASSERT(string.size() == 13);
}

BOOST_AUTO_TEST_CASE(StringViewConstructionFromTwoPointers)
{
    auto str = "Hello, World!"s;
    DEBUG_VAR string = string_view{ str.data(), str.data() + str.size() };// NOLINT
    BOOST_ASSERT(string.size() == 13);
}
