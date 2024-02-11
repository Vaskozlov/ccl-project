#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

TEST_CASE("SpecialItem", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::SpecialItems{};
    const auto text_iterator = text::TextIterator{"+"};

    special_items.specialItems.emplace_back(
        text::TextIterator{R"(!"+")"}, special_items, debug::FirstUsableToken);
    REQUIRE(special_items.checkForSpecial(text_iterator.fork()));
}
