#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/dot_item.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_SUITE(DotItemPostfix)

BOOST_AUTO_TEST_CASE(TwoPostfixes)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"([a-z]+[_]p"test"p)" }, 0, shared);
    DEBUG_VAR &items = dot_item.getItems();

    BOOST_ASSERT(not items[0]->hasPrefix());
    BOOST_ASSERT(not items[0]->hasPostfix());

    BOOST_ASSERT(items[1]->hasPostfix());
    BOOST_ASSERT(items[2]->hasPostfix());
}

BOOST_AUTO_TEST_CASE(WrongPostfixCreation)
{
    auto shared = AnalysisShared{};

    BOOST_CHECK_EXCEPTION(
        DotItem(TextIterator{ u8R"([a-z]+[_]p"test")" }, 0, shared), DotItemException,
        [](const DotItemException &exception) {
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
