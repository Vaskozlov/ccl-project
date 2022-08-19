#include <boost/test/unit_test.hpp>
#include <cerberus/string_view.hpp>
#include <string>

using namespace cerb;
using namespace std::string_literals;

BOOST_AUTO_TEST_CASE(StringViewConstructionEmptyArray)
{
    auto string = BasicStringView<char>{ "" };
    BOOST_ASSERT(string.empty());
}

BOOST_AUTO_TEST_CASE(StringViewConstructionArray)
{
    auto string = BasicStringView<char>{ "Hello, World!" };
    BOOST_ASSERT(string.size() == 13);
}

BOOST_AUTO_TEST_CASE(StringViewConstructionEmptyPointer)
{
    const char *str = "";
    auto string = BasicStringView<char>{ str };
    BOOST_ASSERT(string.empty());
}

BOOST_AUTO_TEST_CASE(StringViewConstructionPointer)
{
    const char *str = "Hello, World!";
    auto string = BasicStringView<char>{ str };
    BOOST_ASSERT(string.size() == 13);
}

BOOST_AUTO_TEST_CASE(StringViewConstructionFromTwoPointers)
{
    auto str = "Hello, World!"s;
    auto string = BasicStringView<char>{ str.data(), str.data() + str.size() };// NOLINT
    BOOST_ASSERT(string.size() == 13);
}
