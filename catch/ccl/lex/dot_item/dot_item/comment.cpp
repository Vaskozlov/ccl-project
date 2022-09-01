#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_SUITE(ContainerComment)

BOOST_AUTO_TEST_CASE(SingleLineComment)
{
    auto shared = AnalysisShared{};
    DEBUG_VAR container = Container(TextIterator{ R"("#"co)" }, 2, shared);

    BOOST_ASSERT(shared.comment_tokens.single_line == "#");
}

BOOST_AUTO_TEST_CASE(MultiLineComment)
{
    auto shared = AnalysisShared{};
    DEBUG_VAR container = Container(TextIterator{ R"("/*:*/"co)" }, 2, shared);

    BOOST_ASSERT(shared.comment_tokens.multiline_begin == "/*");
    BOOST_ASSERT(shared.comment_tokens.multiline_end == "*/");
}

BOOST_AUTO_TEST_SUITE_END()
