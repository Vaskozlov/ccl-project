#include <cerberus/debug/debug_file.hpp>
#include <cerberus/format/core/string_splitter.hpp>

using namespace cerb;
using namespace cerb::fmt::core;
using namespace cerb::string_view_literals;

template<ConstString String>
constexpr auto testSplitter(const std::initializer_list<u8string_view> &blocks) -> bool
{
    auto index = 0_ZU;
    auto split_string = splitString<String>();

    for (const auto &block : blocks) {
        assertEqual(block, split_string[index]);
        ++index;
    }

    return true;
}

static_assert(testSplitter<u8"">({ u8"" }));
static_assert(testSplitter<u8"Hello!">({ u8"Hello!" }));
static_assert(testSplitter<u8"Hello {}, {}!">({ u8"Hello ", u8", ", u8"!" }));
