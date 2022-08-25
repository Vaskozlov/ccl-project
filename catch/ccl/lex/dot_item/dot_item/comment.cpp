#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/dot_item.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_SUITE(DotItemComment)

BOOST_AUTO_TEST_CASE(SingleLineComment)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"("#"co)" }, 0, shared);

    BOOST_ASSERT(shared.comment_tokens.single_line == u8"#");
}

BOOST_AUTO_TEST_CASE(MultiLineComment)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"("/*:*/"co)" }, 0, shared);

    BOOST_ASSERT(shared.comment_tokens.multiline_begin == u8"/*");
    BOOST_ASSERT(shared.comment_tokens.multiline_end == u8"*/");
}

BOOST_AUTO_TEST_SUITE_END()
