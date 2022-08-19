#include <cerberus/debug/debug_file.hpp>
#include <cerberus/utf_set.hpp>

// NOLINTBEGIN

BOOST_AUTO_TEST_SUITE(UtfSet)

BOOST_AUTO_TEST_CASE(UtfSetAsciiTest)
{
    auto set = cerb::UtfSet{};

    set.set(0);
    BOOST_ASSERT(set.at(0));

    set.set(0, false);
    BOOST_ASSERT(not set.at(0));

    set.set(127);
    BOOST_ASSERT(set.at(127));

    set.set(10, 20, true);

    for (char32_t i = 10; i <= 20; ++i) {
        BOOST_ASSERT(set.at(i));
    }

    BOOST_ASSERT(not set.at(21));
    set.set(10, 20, false);

    for (char32_t i = 10; i <= 20; ++i) {
        BOOST_ASSERT(not set.at(i));
    }
}

BOOST_AUTO_TEST_CASE(UtfSetUtfTest)
{
    auto utf_set = cerb::UtfSet{};

    utf_set.set(U'\uFFFF');
    BOOST_ASSERT(utf_set.at(U'\uFFFF'));

    utf_set.set(U'\uFFFF', false);
    BOOST_ASSERT(not utf_set.at(U'\uFFFF'));

    utf_set.set(U'\u10FF', U'\u1200', true);

    for (char32_t i = U'\u10FF'; i <= U'\u1200'; ++i) {
        BOOST_ASSERT(utf_set.at(i));
    }

    BOOST_ASSERT(not utf_set.at(U'\u1201'));
    utf_set.set(U'\u10FF', U'\u1200', false);

    for (char32_t i = U'\u10FF'; i <= U'\u1200'; ++i) {
        BOOST_ASSERT(not utf_set.at(i));
    }
}
// NOLINTEND

BOOST_AUTO_TEST_SUITE_END()
