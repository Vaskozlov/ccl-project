#include <boost/test/unit_test.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

BOOST_AUTO_TEST_SUITE(DotItemTerminal)

BOOST_AUTO_TEST_CASE(DotItemTerminalCreation)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"('+' '-')" }, 0, shared);

    BOOST_ASSERT(shared.terminals.size() == 2);
    BOOST_ASSERT(shared.terminals.matches(u8"+"));
    BOOST_ASSERT(shared.terminals.matches(u8"-"));
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
                u8"unable to create terminal: terminals cannot coexist with other items");
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
                u8"unable to create union: terminals cannot coexist with other items");
            return true;
        });
}

BOOST_AUTO_TEST_SUITE_END()
