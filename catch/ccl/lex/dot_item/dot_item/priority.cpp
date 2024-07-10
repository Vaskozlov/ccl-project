#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>

TEST_CASE("TestBindingPowerOnUnion", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::AnyPlaceItems{};
    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});

    auto container = lexer::dot_item::Container(
        empty_lexical_analyzer, text::TextIterator{R"([a-z]*p_100")"}, special_items,
        debug::FirstUsableToken, true);

    DEBUG_VAR items = container.getItems();
    REQUIRE(items.back()->getBindingPower() == 100);
}
