#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/dot_item.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_SUITE(DotItemTerminal)

BOOST_AUTO_TEST_CASE(DotItemTerminalCreation)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"('+' '-')" }, 0, shared);

    BOOST_ASSERT(shared.special_tokens.size() == 2);
    BOOST_ASSERT(shared.special_tokens.matches(u8"+"));
    BOOST_ASSERT(shared.special_tokens.matches(u8"-"));
}

BOOST_AUTO_TEST_CASE(DotItemWrongTerminalCreationAfterUnion)
{
    auto shared = AnalysisShared{};

    BOOST_CHECK_EXCEPTION(
        DotItem(TextIterator{ u8R"([] '+' )" }, 0, shared), DotItemException,
        [](const DotItemException &exception) {
            BOOST_CHECK_EQUAL(exception.getColumn(), 4);
            BOOST_ASSERT(
                exception.getMessage() ==
                u8"unable to create special symbol: special symbols cannot coexist with other items");
            return true;
        });
}

BOOST_AUTO_TEST_CASE(DotItemWrongTerminalCreationBeforeUnion)
{
    auto shared = AnalysisShared{};

    BOOST_CHECK_EXCEPTION(
        DotItem(TextIterator{ u8R"('+' [])" }, 0, shared), DotItemException,
        [](const DotItemException &exception) {
            BOOST_CHECK_EQUAL(exception.getColumn(), 5);
            BOOST_ASSERT(
                exception.getMessage() ==
                u8"unable to create union: special symbols cannot coexist with other items");
            return true;
        });
}

BOOST_AUTO_TEST_SUITE_END()
