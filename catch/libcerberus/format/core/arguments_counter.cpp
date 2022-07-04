#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/arguments_counter.hpp>

// NOLINTBEGIN

using namespace std::string_view_literals;

template<cerb::CharacterLiteral CharT>
using ArgumentsCounter = cerb::fmt::core::ArgumentsCounter<CharT>;

template class cerb::fmt::core::ArgumentsCounter<char>;
template class cerb::fmt::core::ArgumentsCounter<char16_t>;

DEBUG_SCOPE
{
    STATIC_VARIABLE ArgumentsCounter<char> splitter{ "" };
    STATIC_ASSERT_EQ(splitter.get(), static_cast<size_t>(0));
    return {};
}


DEBUG_SCOPE
{
    STATIC_VARIABLE ArgumentsCounter<char> splitter{ "Hello!{}" };
    STATIC_ASSERT_EQ(splitter.get(), static_cast<size_t>(1));
    return {};
}

DEBUG_SCOPE
{
    STATIC_VARIABLE ArgumentsCounter<char> splitter{ "Hello {}, { }!" };
    STATIC_ASSERT_EQ(splitter.get(), static_cast<size_t>(2));
    return {};
}

DEBUG_SCOPE
{
    STATIC_VARIABLE ArgumentsCounter<char> splitter{ "Hello {}{{, {}}}!" };
    STATIC_ASSERT_EQ(splitter.get(), static_cast<size_t>(2));
    return {};
}

// NOLINTEND
