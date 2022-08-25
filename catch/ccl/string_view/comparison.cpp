#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace string_view_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_SUITE(StringViewComparison)

BOOST_AUTO_TEST_CASE(StringViewComparisonEqual)
{
    DEBUG_VAR std_string = "Hello, World!"sv;
    DEBUG_VAR cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string == std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonLessFirstLess)
{
    DEBUG_VAR std_string = "Hello, World!"sv;
    DEBUG_VAR cerb_string = "hello, World!"_sv;

    BOOST_ASSERT(cerb_string > std_string);
    BOOST_ASSERT(cerb_string >= std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonLessFirstGreater)
{
    DEBUG_VAR std_string = "hello, World!"sv;
    DEBUG_VAR cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string < std_string);
    BOOST_ASSERT(cerb_string <= std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonGreaterFirstLess)
{
    DEBUG_VAR std_string = "Hello, World!"sv;
    DEBUG_VAR cerb_string = "hello, World!"_sv;

    BOOST_ASSERT(cerb_string > std_string);
    BOOST_ASSERT(cerb_string >= std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonGreaterFirstGreater)
{
    DEBUG_VAR std_string = "Hello, World"sv;
    DEBUG_VAR cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string > std_string);
    BOOST_ASSERT(cerb_string >= std_string);
}

BOOST_AUTO_TEST_SUITE_END()
