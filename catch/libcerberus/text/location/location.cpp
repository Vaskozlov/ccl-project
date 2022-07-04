#include "cerberus/text/location.hpp"
#include "cerberus/debug/debug.hpp"

using Location = cerb::text::Location<>;

// NOLINTBEGIN

DEBUG_SCOPE
{
    Location location{ "none" };

    ASSERT_EQ(location.getLine(), 1U);
    ASSERT_EQ(location.getColumn(), 1U);
    ASSERT_EQ(location.getOffset(), 0U);
    ASSERT_EQ(location.getFilename(), "none");

    location.nextChar();

    ASSERT_EQ(location.getLine(), 1U);
    ASSERT_EQ(location.getColumn(), 2U);
    ASSERT_EQ(location.getOffset(), 1U);

    location.nextLine();

    ASSERT_EQ(location.getLine(), 2U);
    ASSERT_EQ(location.getColumn(), 1U);
    ASSERT_EQ(location.getOffset(), 2U);

    return {};
}

// NOLINTEND
