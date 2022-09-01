#include <ccl/const_string.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;

template<ConstString String>
static auto testConstString(string_view input) -> bool
{
    DEBUG_VAR test_string = input;

    BOOST_ASSERT(String.empty() == test_string.empty());
    BOOST_ASSERT(String.size() == test_string.size());
    BOOST_ASSERT(static_cast<string_view>(String) == test_string);

    BOOST_ASSERT(std::ranges::equal(String, test_string));
    BOOST_ASSERT(std::equal(String.begin(), String.end(), test_string.begin()));

    return true;
}

BOOST_AUTO_TEST_SUITE(ConstString)

BOOST_AUTO_TEST_CASE(ConstStringEmptyInput)
{
    BOOST_ASSERT(testConstString<"">(""));
}

BOOST_AUTO_TEST_CASE(ConstStringSingleBasicInput)
{
    BOOST_ASSERT(testConstString<"Hello, World!">("Hello, World!"));
}

BOOST_AUTO_TEST_SUITE_END()
