#include <cerberus/const_string.hpp>
#include <cerberus/debug/debug_file.hpp>

template<cerb::ConstString String>
static auto testConstString(cerb::u8string_view input) -> bool
{
    auto test_string = input;

    BOOST_ASSERT(String.empty() == test_string.empty());
    BOOST_ASSERT(String.size() == test_string.size());
    BOOST_ASSERT(static_cast<cerb::u8string_view>(String) == test_string);

    BOOST_ASSERT(std::ranges::equal(String, test_string));
    BOOST_ASSERT(std::equal(String.begin(), String.end(), test_string.begin()));

    return true;
}

BOOST_AUTO_TEST_SUITE(ConstString)

BOOST_AUTO_TEST_CASE(ConstStringEmptyInput)
{
    BOOST_ASSERT(testConstString<u8"">(u8""));
}

BOOST_AUTO_TEST_CASE(ConstStringSingleBasicInput)
{
    BOOST_ASSERT(testConstString<u8"Hello, World!">(u8"Hello, World!"));
}

BOOST_AUTO_TEST_SUITE_END()
