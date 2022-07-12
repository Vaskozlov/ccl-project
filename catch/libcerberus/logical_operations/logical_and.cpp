#include <cerberus/core/logical_operations.hpp>
#include <cerberus/debug/debug.hpp>

using namespace cerb;

STATIC_ASSERT_EQ(land(true), true);
STATIC_ASSERT_EQ(land(false), false);

STATIC_ASSERT_EQ(land(true, true), true);
STATIC_ASSERT_EQ(land(true, false), false);

STATIC_ASSERT_EQ(land(true, true, true), true);
STATIC_ASSERT_EQ(land(true, true, false), false);
STATIC_ASSERT_EQ(land(false, false, false), false);
