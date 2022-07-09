#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/arguments_counter.hpp>

using namespace cerb::integral_literals;
using ArgumentsCounter = cerb::fmt::core::ArgumentsCounter<char16_t>;

CONSTEXPR_TEST
{
    auto splitter = ArgumentsCounter{ u"" };
    ASSERT_EQ(splitter.get(), 0_ZU);
    return {};
}
();

CONSTEXPR_TEST
{
    auto splitter = ArgumentsCounter{ u"Hello!{}" };
    ASSERT_EQ(splitter.get(), 1_ZU);
    return {};
}
();

CONSTEXPR_TEST
{
    auto splitter = ArgumentsCounter{ u"Hello {}, { }!" };
    ASSERT_EQ(splitter.get(), 2_ZU);
    return {};
}
();

CONSTEXPR_TEST
{
    auto splitter = ArgumentsCounter{ u"Hello {}{{, {}}}!" };
    ASSERT_EQ(splitter.get(), 2_ZU);
    return {};
}
();
