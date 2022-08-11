#include <cerberus/debug/debug_file.hpp>
#include <cerberus/utf8.hpp>
#include <cerberus/utf_set.hpp>

// NOLINTBEGIN
RUNTIME_TEST
{
    auto utf_set = cerb::UtfSet{};

    utf_set.set(U'\uFFFF');
    assertTrue(utf_set.at(U'\uFFFF'));

    utf_set.set(U'\uFFFF', false);
    assertFalse(utf_set.at(U'\uFFFF'));

    utf_set.set(U'\u10FF', U'\u1200', true);

    for (char32_t i = U'\u10FF'; i <= U'\u1200'; ++i) {
        assertTrue(utf_set.at(i));
    }

    assertFalse(utf_set.at(U'\u1201'));
    utf_set.set(U'\u10FF', U'\u1200', false);

    for (char32_t i = U'\u10FF'; i <= U'\u1200'; ++i) {
        assertFalse(utf_set.at(i));
    }

    return {};
}
();

// NOLINTEND
