#include <cerberus/core/logical_ops.hpp>

using namespace cerb;

static_assert(lor(true));
static_assert(not lor(false));

static_assert(lor(true, true));
static_assert(lor(true, false));
static_assert(not lor(false, false));

static_assert(lor(true, true, true));
static_assert(lor(true, true, false));
static_assert(not lor(false, false, false));
