#include <ccl/debug/debug.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>

TEST_CASE("AnyPlaceItem", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::AnyPlaceItems{};
    const auto text_iterator = text::TextIterator{"+"};
    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});

    special_items.items.emplace_back(isl::makeUnique<lexer::dot_item::Container>(
        empty_lexical_analyzer, text::TextIterator{R"(!"+")"}, special_items,
        debug::FirstUsableToken));
    REQUIRE(special_items.checkForSpecial(text_iterator.fork()));
}
