#include <ccl/debug/debug_file.hpp>
#include <ccl/format/core/string_splitter.hpp>

using namespace ccl;
using namespace ccl::fmt::core;

template<ConstString String>
auto testSplitter(const std::initializer_list<u8string_view> &blocks) -> bool
{
    DEBUG_VAR index = static_cast<size_t>(0);
    auto split_string = splitString<String>();

    for (DEBUG_VAR &&block : blocks) {
        BOOST_ASSERT(block == split_string[index]);
        ++index;
    }

    return true;
}

BOOST_AUTO_TEST_CASE(FormatSplitString)
{
    BOOST_ASSERT(testSplitter<u8"">({ u8"" }));
    BOOST_ASSERT(testSplitter<u8"Hello!">({ u8"Hello!" }));
    BOOST_ASSERT(testSplitter<u8"Hello {}, {}!">({ u8"Hello ", u8", ", u8"!" }));
}
