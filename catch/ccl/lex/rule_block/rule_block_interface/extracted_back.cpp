#include <ccl/debug/debug.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>

TEST_CASE("TwoExtractableAtTheEnd", "[Container]")
{
    using namespace ccl;

    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});
    auto container = lexer::rule::Container(
        empty_lexical_analyzer, text::TextIterator{R"([a-z]+[_]e"test"e)"}, true, true);

    DEBUG_VAR items = container.getItems();

    REQUIRE(items.at(1)->isExtractable());
    REQUIRE(items.at(2)->isExtractable());
}
