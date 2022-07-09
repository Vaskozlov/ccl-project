#include <cerberus/core/logical_operations.hpp>
#include <cerberus/debug/debug.hpp>

using namespace cerb;

STATIC_ASSERT_EQ(lor(true), true);
STATIC_ASSERT_EQ(lor(false), false);

STATIC_ASSERT_EQ(lor(true, true), true);
STATIC_ASSERT_EQ(lor(true, false), true);
STATIC_ASSERT_EQ(lor(false, false), false);

STATIC_ASSERT_EQ(lor(true, true, true), true);
STATIC_ASSERT_EQ(lor(true, true, false), true);
STATIC_ASSERT_EQ(lor(false, false, false), false);
