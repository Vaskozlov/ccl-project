#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/arguments_counter.hpp>

// NOLINTBEGIN

using namespace cerb::integral_literals;

using ArgumentsCounter = cerb::fmt::core::ArgumentsCounter<char>;

DEBUG_SCOPE
{
    STATIC_VARIABLE auto splitter = ArgumentsCounter{ "" };
    STATIC_ASSERT_EQ(splitter.get(), 0_ZU);
    return {};
}

DEBUG_SCOPE
{
    STATIC_VARIABLE auto splitter = ArgumentsCounter{ "Hello!{}" };
    STATIC_ASSERT_EQ(splitter.get(), 1_ZU);
    return {};
}

DEBUG_SCOPE
{
    STATIC_VARIABLE auto splitter = ArgumentsCounter{ "Hello {}, { }!" };
    STATIC_ASSERT_EQ(splitter.get(), 2_ZU);
    return {};
}

DEBUG_SCOPE
{
    STATIC_VARIABLE auto splitter = ArgumentsCounter{ "Hello {}{{, {}}}!" };
    STATIC_ASSERT_EQ(splitter.get(), 2_ZU);
    return {};
}

// NOLINTEND
