#include "cerberus/format/core/arguments_counter.hpp"

using namespace cerb::fmt::core;
using namespace std::string_view_literals;

constexpr static auto argumentsCounterTest(auto string, size_t n) -> bool
{
    auto counter = ArgumentsCounter{ string };
    return counter.get() == n;
}

static_assert(argumentsCounterTest(""sv, 0));
static_assert(argumentsCounterTest("Hello!{}"sv, 1));
static_assert(argumentsCounterTest("Hello {}, { }!"sv, 2));
static_assert(argumentsCounterTest("Hello {}{{, {}}}!"sv, 2));

static_assert(argumentsCounterTest(u""sv, 0));
static_assert(argumentsCounterTest(u"Hello!{}"sv, 1));
static_assert(argumentsCounterTest(u"Hello {}, { }!"sv, 2));
static_assert(argumentsCounterTest(u"Hello {}{{, {}}}!"sv, 2));