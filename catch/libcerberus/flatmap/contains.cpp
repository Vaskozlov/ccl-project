#include <cerberus/debug/debug.hpp>
#include <cerberus/flatmap.hpp>

// NOLINTBEGIN

using Flatmap = cerb::Flatmap<int, int, 10>;
template class cerb::Flatmap<int, int, 10>;

DEBUG_SCOPE
{
    STATIC_VARIABLE auto flatmap = Flatmap{ { 10, 20 }, { 20, 30 }, { 30, 40 } };

    static_assert(flatmap.size() == 3);
    static_assert(flatmap.capacity() == 10);

    static_assert(flatmap.contains(10));
    static_assert(flatmap.contains(20));
    static_assert(flatmap.contains(30));

    static_assert(!flatmap.contains(0));
    static_assert(!flatmap.contains(40));

    return {};
}

// NOLINTEND
