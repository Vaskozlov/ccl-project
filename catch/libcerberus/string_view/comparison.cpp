#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_view.hpp>

using namespace cerb::string_view_literals;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_SUITE(StringViewComparison)

BOOST_AUTO_TEST_CASE(StringViewComparisonEqual)
{
    DEBUG_DECL std_string = "Hello, World!"sv;
    DEBUG_DECL cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string == std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonLessFirstLess)
{
    DEBUG_DECL std_string = "Hello, World!"sv;
    DEBUG_DECL cerb_string = "hello, World!"_sv;

    BOOST_ASSERT(cerb_string > std_string);
    BOOST_ASSERT(cerb_string >= std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonLessFirstGreater)
{
    DEBUG_DECL std_string = "hello, World!"sv;
    DEBUG_DECL cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string < std_string);
    BOOST_ASSERT(cerb_string <= std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonGreaterFirstLess)
{
    DEBUG_DECL std_string = "Hello, World!"sv;
    DEBUG_DECL cerb_string = "hello, World!"_sv;

    BOOST_ASSERT(cerb_string > std_string);
    BOOST_ASSERT(cerb_string >= std_string);
}

BOOST_AUTO_TEST_CASE(StringViewComparisonGreaterFirstGreater)
{
    DEBUG_DECL std_string = "Hello, World"sv;
    DEBUG_DECL cerb_string = "Hello, World!"_sv;

    BOOST_ASSERT(cerb_string > std_string);
    BOOST_ASSERT(cerb_string >= std_string);
}

BOOST_AUTO_TEST_SUITE_END()
