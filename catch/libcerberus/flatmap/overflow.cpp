#include <cerberus/debug/debug.hpp>
#include <cerberus/flatmap.hpp>

// NOLINTBEGIN

using Flatmap = cerb::Flatmap<int, int, 10>;

DEBUG_SCOPE
{
    auto flatmap = Flatmap{ { 10, 20 }, { 20, 30 }, { 30, 40 }, { 30, 40 },  { 40, 50 },
                            { 60, 70 }, { 70, 80 }, { 80, 80 }, { 90, 100 }, { 100, 110 } };

    static_assert(flatmap.capacity() == 10);

    ERROR_EXPECTED(flatmap.insert(110, 120), cerb::OutOfRange, "flatmap is full");

    return {};
};

// NOLINTEND
