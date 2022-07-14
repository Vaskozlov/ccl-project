#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/arguments_counter.hpp>

using namespace cerb::debug;
using namespace cerb::fmt::core;
using namespace cerb::string_view_literals;

constexpr static auto counterTest(auto string, size_t n) -> bool
{
    auto counter = ArgumentsCounter{ string };
    return counter.get() == n;
}

CONSTEXPR_TEST
{
    assertTrue(counterTest(""_sv, 0));
    assertTrue(counterTest("Hello!{}"_sv, 1));
    assertTrue(counterTest("Hello {}, { }!"_sv, 2));
    assertTrue(counterTest("Hello {}{{, {}}}!_"_sv, 2));

    assertTrue(counterTest(u""_sv, 0));
    assertTrue(counterTest(u"Hello!{}"_sv, 1));
    assertTrue(counterTest(u"Hello {}, { }!"_sv, 2));
    assertTrue(counterTest(u"Hello {}{{, {}}}!"_sv, 2));

    return {};
}
();
