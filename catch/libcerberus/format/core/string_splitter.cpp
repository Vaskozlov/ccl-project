#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/string_splitter.hpp>

using namespace cerb::debug;
using namespace cerb::fmt::core;
using namespace cerb::integral_literals;
using namespace std::string_view_literals;

template<cerb::TemplateString String, cerb::CharacterLiteral CharT>
constexpr auto testSplitter(const std::initializer_list<std::basic_string_view<CharT>> &blocks)
    -> bool
{
    auto index = 0_ZU;
    auto split_string = splitString<String>();

    for (const auto &block : blocks) {
        assertEqual(block, split_string[index]);
        ++index;
    }

    return true;
}

static_assert(testSplitter<"">({ ""sv }));
static_assert(testSplitter<"Hello!">({ "Hello!"sv }));
static_assert(testSplitter<"Hello {}, {}!">({ "Hello "sv, ", "sv, "!"sv }));

static_assert(testSplitter<u"">({ u""sv }));
static_assert(testSplitter<u"Hello!">({ u"Hello!"sv }));
static_assert(testSplitter<u"Hello {}, {}!">({ u"Hello "sv, u", "sv, u"!"sv }));
