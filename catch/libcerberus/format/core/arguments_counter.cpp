#include <cerberus/debug/debug_file.hpp>
#include <cerberus/format/core/arguments_counter.hpp>

using namespace cerb::fmt::core;
using namespace cerb::string_view_literals;

constexpr static auto counterTest(cerb::u8string_view string, size_t n) -> bool
{
    auto counter = ArgumentsCounter{ string };
    return counter.get() == n;
}

CONSTEXPR_TEST
{
    assertTrue(counterTest(u8"", 0));
    assertTrue(counterTest(u8"Hello!{}", 1));
    assertTrue(counterTest(u8"Hello {}, { }!", 2));
    assertTrue(counterTest(u8"Hello {}{{, {}}}!_", 2));
    return {};
}
();
