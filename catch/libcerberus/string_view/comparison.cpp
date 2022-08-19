#include <boost/test/unit_test.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_SUITE(StringViewComparison)

BOOST_AUTO_TEST_CASE(StringViewComparisonEqual)
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string == std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonLessFirstLess)
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "hello, World!"_sv;

    BOOST_ASSERT(cerb_string > std_string);
    BOOST_ASSERT(cerb_string >= std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonLessFirstGreater)
{
    auto std_string = "hello, World!"sv;
    auto cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string < std_string);
    BOOST_ASSERT(cerb_string <= std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonGreaterFirstLess)
{
    auto std_string = "Hello, World!"sv;
    auto cerb_string = "hello, World!"_sv;

    BOOST_ASSERT(cerb_string > std_string);
    BOOST_ASSERT(cerb_string >= std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonGreaterFirstGreater)
{
    auto std_string = "Hello, World"sv;
    auto cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string > std_string);
    BOOST_ASSERT(cerb_string >= std_string);
}

BOOST_AUTO_TEST_SUITE_END()
