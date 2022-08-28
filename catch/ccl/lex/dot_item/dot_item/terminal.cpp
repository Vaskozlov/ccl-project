#include <ccl/lex/dot_item/container.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_SUITE(ContainerSpecialTokens)

BOOST_AUTO_TEST_CASE(ContainerTerminalCreation)
{
    auto shared = AnalysisShared{};
    auto container = Container(TextIterator{ u8R"('+' '-')" }, 0, shared);

    BOOST_ASSERT(shared.special_tokens.size() == 2);
    BOOST_ASSERT(shared.special_tokens.matches(u8"+"));
    BOOST_ASSERT(shared.special_tokens.matches(u8"-"));
}

BOOST_AUTO_TEST_CASE(ContainerWrongSpecialTokenCreationAfterUnion)
{
    auto shared = AnalysisShared{};

    BOOST_CHECK_EXCEPTION(
        Container(TextIterator{ u8R"([] '+' )" }, 0, shared), ContainerException,
        [](const ContainerException &exception) {
            BOOST_CHECK_EQUAL(exception.getColumn(), 4);
            BOOST_ASSERT(
                exception.getMessage() ==
                u8"unable to create special symbol: special symbols cannot coexist with other items");
            return true;
        });
}

BOOST_AUTO_TEST_CASE(ContainerWrongSpecialTokenCreationBeforeUnion)
{
    auto shared = AnalysisShared{};

    BOOST_CHECK_EXCEPTION(
        Container(TextIterator{ u8R"('+' [])" }, 0, shared), ContainerException,
        [](const ContainerException &exception) {
            BOOST_CHECK_EQUAL(exception.getColumn(), 5);
            BOOST_ASSERT(
                exception.getMessage() ==
                u8"unable to create union: special symbols cannot coexist with other items");
            return true;
        });
}

BOOST_AUTO_TEST_SUITE_END()
