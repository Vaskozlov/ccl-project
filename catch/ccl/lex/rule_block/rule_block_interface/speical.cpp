#include <ccl/debug/debug.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>

TEST_CASE("AnyPlaceItem", "[Container]")
{
    using namespace ccl;

    const auto text_iterator = text::TextIterator{"+"};
    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});
    auto &special_items = empty_lexical_analyzer.shareAnyPlaceItems();

    special_items.items.emplace_back(isl::makeUnique<lexer::rule::Container>(
        empty_lexical_analyzer, text::TextIterator{R"(!"+")"}));

    REQUIRE(special_items.checkForSpecial(text_iterator.fork()));
}
