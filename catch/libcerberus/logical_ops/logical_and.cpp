#include <cerberus/core/logical_ops.hpp>

using namespace cerb;

static_assert(land(true));
static_assert(not land(false));

static_assert(land(true, true));
static_assert(not land(true, false));

static_assert(land(true, true, true));
static_assert(not land(true, true, false));
static_assert(not land(false, false, false));
