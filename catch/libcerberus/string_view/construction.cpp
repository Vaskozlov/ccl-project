#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>
#include <string>

using namespace cerb;
using namespace std::string_literals;

BOOST_AUTO_TEST_CASE(StringViewConstructionEmptyArray)
{
    DEBUG_DECL string = BasicStringView<char>{ "" };
    BOOST_ASSERT(string.empty());
}

BOOST_AUTO_TEST_CASE(StringViewConstructionArray)
{
    DEBUG_DECL string = BasicStringView<char>{ "Hello, World!" };
    BOOST_ASSERT(string.size() == 13);
}

BOOST_AUTO_TEST_CASE(StringViewConstructionEmptyPointer)
{
    const char *str = "";
    DEBUG_DECL string = BasicStringView<char>{ str };
    BOOST_ASSERT(string.empty());
}

BOOST_AUTO_TEST_CASE(StringViewConstructionPointer)
{
    const char *str = "Hello, World!";
    DEBUG_DECL string = BasicStringView<char>{ str };
    BOOST_ASSERT(string.size() == 13);
}

BOOST_AUTO_TEST_CASE(StringViewConstructionFromTwoPointers)
{
    auto str = "Hello, World!"s;
    DEBUG_DECL string = BasicStringView<char>{ str.data(), str.data() + str.size() };// NOLINT
    BOOST_ASSERT(string.size() == 13);
}
