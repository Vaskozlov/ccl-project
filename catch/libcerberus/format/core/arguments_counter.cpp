#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/arguments_counter.hpp>

using namespace cerb::debug;
using namespace cerb::fmt::core;
using namespace std::string_view_literals;
using namespace cerb::string_view_literals;

constexpr static auto argumentsCounterTest(auto string, size_t n) -> bool
{
    auto counter = ArgumentsCounter{ string };
    return counter.get() == n;
}

STRING_TEST{
    assertTrue(argumentsCounterTest(""_sv, 0));
    assertTrue(argumentsCounterTest("Hello!{}"_sv, 1));
    assertTrue(argumentsCounterTest("Hello {}, { }!"_sv, 2));
    assertTrue(argumentsCounterTest("Hello {}{{, {}}}!_"_sv, 2));

    assertTrue(argumentsCounterTest(u""_sv, 0));
    assertTrue(argumentsCounterTest(u"Hello!{}"_sv, 1));
    assertTrue(argumentsCounterTest(u"Hello {}, { }!"_sv, 2));
    assertTrue(argumentsCounterTest(u"Hello {}{{, {}}}!"_sv, 2));

    return {};
}();
