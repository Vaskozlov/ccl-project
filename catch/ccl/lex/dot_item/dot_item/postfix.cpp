#include <ccl/lex/dot_item/container.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_SUITE(ContainerPostfix)

BOOST_AUTO_TEST_CASE(TwoPostfixes)
{
    auto shared = AnalysisShared{};
    auto container = Container(TextIterator{ u8R"([a-z]+[_]p"test"p)" }, 0, shared);
    DEBUG_VAR &items = container.getItems();

    BOOST_ASSERT(not items[0]->hasPrefix());
    BOOST_ASSERT(not items[0]->hasPostfix());

    BOOST_ASSERT(items[1]->hasPostfix());
    BOOST_ASSERT(items[2]->hasPostfix());
}

BOOST_AUTO_TEST_CASE(WrongPostfixCreation)
{
    auto shared = AnalysisShared{};

    BOOST_CHECK_EXCEPTION(
        Container(TextIterator{ u8R"([a-z]+[_]p"test")" }, 0, shared), ContainerException,
        [](const ContainerException &exception) {
            BOOST_CHECK_EQUAL(exception.getColumn(), 16);
            BOOST_ASSERT(
                exception.getMessage() ==
                u8"unable to apply: item without postfix modifier exists after items with it");
            BOOST_ASSERT(
                exception.getSuggestion() ==
                u8"add postfix modifier to the last item\n[a-z]+[_]p\"test\"p");
            return true;
        });
}

BOOST_AUTO_TEST_SUITE_END()
