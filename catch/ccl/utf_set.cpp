#include <ccl/debug/debug_file.hpp>
#include <ccl/utf_set.hpp>

using namespace ccl;

// NOLINTBEGIN

BOOST_AUTO_TEST_SUITE(UtfSet)

BOOST_AUTO_TEST_CASE(UtfSetAsciiTest)
{
    auto set = ccl::UtfSet{};

    set.set(0);
    BOOST_ASSERT(set[0]);

    set.set(0, false);
    BOOST_ASSERT(!set[0]);

    set.set(127);
    BOOST_ASSERT(set[127]);

    set.set(10, 20, true);

    for (char32_t i = 10; i <= 20; ++i) {
        BOOST_ASSERT(set[i]);
    }

    BOOST_ASSERT(!set[21]);
    set.set(10, 20, false);

    for (char32_t i = 10; i <= 20; ++i) {
        BOOST_ASSERT(!set[i]);
    }
}

BOOST_AUTO_TEST_CASE(UtfSetUtfTest)
{
    auto utf_set = ccl::UtfSet{};

    utf_set.set(U'\uFFFF');
    BOOST_ASSERT(utf_set[U'\uFFFF']);

    utf_set.set(U'\uFFFF', false);
    BOOST_ASSERT(!utf_set[U'\uFFFF']);

    utf_set.set(U'\u10FF', U'\u1200', true);

    for (char32_t i = U'\u10FF'; i <= U'\u1200'; ++i) {
        BOOST_ASSERT(utf_set[i]);
    }

    BOOST_ASSERT(!utf_set[U'\u1201']);
    utf_set.set(U'\u10FF', U'\u1200', false);

    for (char32_t i = U'\u10FF'; i <= U'\u1200'; ++i) {
        BOOST_ASSERT(!utf_set[i]);
    }
}
// NOLINTEND

BOOST_AUTO_TEST_SUITE_END()
