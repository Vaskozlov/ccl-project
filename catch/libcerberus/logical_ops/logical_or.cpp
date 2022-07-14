#include <cerberus/core/logical_ops.hpp>
#include <cerberus/debug/debug.hpp>

using namespace cerb;

static_assert(lor(true) == true);
static_assert(lor(false) == false);

static_assert(lor(true, true) == true);
static_assert(lor(true, false) == true);
static_assert(lor(false, false) == false);

static_assert(lor(true, true, true) == true);
static_assert(lor(true, true, false) == true);
static_assert(lor(false, false, false) == false);
