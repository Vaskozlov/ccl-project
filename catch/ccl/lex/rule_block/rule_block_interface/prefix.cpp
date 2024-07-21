#include <ccl/debug/debug.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>

TEST_CASE("ContainerTwoPrefixesCreation", "[Container]")
{
    using namespace ccl;

    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});

    auto container = lexer::rule::Container(
        empty_lexical_analyzer, text::TextIterator{R"([a-z]*p[_]p"test")"}, debug::FirstUsableToken,
        true);
    DEBUG_VAR items = container.getItems();

    REQUIRE(items.at(0)->hasPrefix());
    REQUIRE(items.at(1)->hasPrefix());
    REQUIRE(!items.at(2)->hasPrefix());
}
