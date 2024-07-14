#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

TEST_CASE("ContainerTwoPrefixesCreation", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::AnyPlaceItems{};
    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});

    auto container = lexer::dot_item::Container(
        empty_lexical_analyzer, text::TextIterator{R"([a-z]*p[_]p"test")"}, special_items,
        debug::FirstUsableToken, true);
    DEBUG_VAR items = container.getItems();

    REQUIRE(items.at(0)->hasPrefix());
    REQUIRE(items.at(1)->hasPrefix());
    REQUIRE(!items.at(2)->hasPrefix());
}
