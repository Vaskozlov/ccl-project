#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator_modules/location.hpp>

using Location = cerb::text::Location<>;

// NOLINTBEGIN

DEBUG_SCOPE
{
    Location location{ "none" };

    ASSERT_EQ(location.getLine(), 1U);
    ASSERT_EQ(location.getColumn(), 1U);
    ASSERT_EQ(location.getFilename(), "none");

    location.next('a');

    ASSERT_EQ(location.getLine(), 1U);
    ASSERT_EQ(location.getColumn(), 2U);

    location.next('\n');

    ASSERT_EQ(location.getLine(), 2U);
    ASSERT_EQ(location.getColumn(), 1U);

    return {};
}

// NOLINTEND
