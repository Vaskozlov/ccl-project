#include <ccl/debug/debug.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>

TEST_CASE("TwoPostfixes", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::AnyPlaceItems{};
    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});
    auto container = lexer::rule::Container(
        empty_lexical_analyzer,
        text::TextIterator{R"([a-z]+[_]p"test"p)"},
        special_items,
        lexer::ReservedTokenMaxValue + 1,
        true);
    DEBUG_VAR items = container.getItems();

    REQUIRE(!items.at(0)->hasPrefix());
    REQUIRE(!items.at(0)->hasPostfix());

    REQUIRE(items.at(1)->hasPostfix());
    REQUIRE(items.at(2)->hasPostfix());
}

TEST_CASE("WrongPostfixCreation", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::AnyPlaceItems{};
    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});

    REQUIRE_THROWS_AS(
        lexer::rule::Container(
            empty_lexical_analyzer,
            text::TextIterator{R"([a-z]+[_]p"test")"},
            special_items,
            lexer::ReservedTokenMaxValue + 1,
            true),
        text::TextIteratorException);
}
