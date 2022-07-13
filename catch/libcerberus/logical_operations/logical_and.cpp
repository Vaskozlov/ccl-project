#include <cerberus/core/logical_operations.hpp>
#include <cerberus/debug/debug.hpp>

using namespace cerb;

static_assert(land(true) == true);
static_assert(land(false) == false);

static_assert(land(true, true) == true);
static_assert(land(true, false) == false);

static_assert(land(true, true, true) == true);
static_assert(land(true, true, false) == false);
static_assert(land(false, false, false) == false);
