#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/string_splitter.hpp>

using namespace cerb::debug;
using namespace cerb::fmt::core;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

template<cerb::TemplateString String, cerb::CharacterLiteral CharT>
constexpr auto testSplitter(const std::initializer_list<cerb::BasicStringView<CharT>> &blocks)
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

static_assert(testSplitter<"">({ ""_sv }));
static_assert(testSplitter<"Hello!">({ "Hello!"_sv }));
static_assert(testSplitter<"Hello {}, {}!">({ "Hello "_sv, ", "_sv, "!"_sv }));

static_assert(testSplitter<u"">({ u""_sv }));
static_assert(testSplitter<u"Hello!">({ u"Hello!"_sv }));
static_assert(testSplitter<u"Hello {}, {}!">({ u"Hello "_sv, u", "_sv, u"!"_sv }));
