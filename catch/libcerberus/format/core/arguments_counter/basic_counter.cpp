#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/arguments_counter.hpp>

using namespace cerb::integral_literals;
using ArgumentsCounter = cerb::fmt::core::ArgumentsCounter<char>;

CONSTEXPR_TEST
{
    auto splitter = ArgumentsCounter{ "" };
    ASSERT_EQ(splitter.get(), 0_ZU);
    return {};
}
();

CONSTEXPR_TEST
{
    auto splitter = ArgumentsCounter{ "Hello!{}" };
    ASSERT_EQ(splitter.get(), 1_ZU);
    return {};
}
();

CONSTEXPR_TEST
{
    auto splitter = ArgumentsCounter{ "Hello {}, { }!" };
    ASSERT_EQ(splitter.get(), 2_ZU);
    return {};
}
();

CONSTEXPR_TEST
{
    auto splitter = ArgumentsCounter{ "Hello {}{{, {}}}!" };
    ASSERT_EQ(splitter.get(), 2_ZU);
    return {};
}
();
