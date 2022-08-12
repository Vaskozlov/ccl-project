#include <cerberus/debug/debug_file.hpp>
#include <cerberus/utf_set.hpp>

// NOLINTBEGIN
RUNTIME_TEST
{
    auto set = cerb::UtfSet{};

    set.set(0);
    assertTrue(set.at(0));

    set.set(0, false);
    assertFalse(set.at(0));

    set.set(127);
    assertTrue(set.at(127));

    set.set(10, 20, true);

    for (char32_t i = 10; i <= 20; ++i) {
        assertTrue(set.at(i));
    }

    assertFalse(set.at(21));
    set.set(10, 20, false);

    for (char32_t i = 10; i <= 20; ++i) {
        assertFalse(set.at(i));
    }

    return {};
}
();

// NOLINTEND
