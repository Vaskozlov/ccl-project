#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_SUITE(ContainerPostfix)

BOOST_AUTO_TEST_CASE(TwoPostfixes)
{
    auto special_items = SpecialItems{};
    auto container = Container(TextIterator{R"([a-z]+[_]p"test"p)"}, special_items, 2, true);
    DEBUG_VAR &items = container.getItems();

    BOOST_ASSERT(!items[0]->hasPrefix());
    BOOST_ASSERT(!items[0]->hasPostfix());

    BOOST_ASSERT(items[1]->hasPostfix());
    BOOST_ASSERT(items[2]->hasPostfix());
}

BOOST_AUTO_TEST_CASE(WrongPostfixCreation)
{
    auto special_items = SpecialItems{};

    BOOST_CHECK_EXCEPTION(
        Container(TextIterator{R"([a-z]+[_]p"test")"}, special_items, 2, true),
        text::TextIteratorException, [](const text::TextIteratorException &exception) {
            BOOST_CHECK_EQUAL(exception.getColumn(), 16);
            BOOST_ASSERT(
                exception.getMessage() ==
                "unable to apply: item without postfix modifier exists after items with it");
            return true;
        });
}

BOOST_AUTO_TEST_SUITE_END()
