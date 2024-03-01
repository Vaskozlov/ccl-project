#include <ccl/debug/debug_file.hpp>
import ccl.lexer;
import ccl.debug;

TEST_CASE("SpecialItem", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::dot_item::SpecialItems{};
    const auto text_iterator = text::TextIterator{"+"};

    special_items.specialItems.emplace_back(
        text::TextIterator{R"(!"+")"}, special_items, debug::FirstUsableToken);
    REQUIRE(special_items.checkForSpecial(text_iterator.fork()));
}
