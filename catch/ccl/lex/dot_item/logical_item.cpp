#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace text;
using namespace lex::dot_item;

TEST_CASE("LogicalItemWith2Elements", "[LogicalItem]")
{
    auto text_iterator = TextIterator{"[1] | \"2\""};
    auto special_items = lex::SpecialItems{};

    auto container = Container(text_iterator, special_items, 2);
    const auto &items = container.getItems();
    const auto *logical_unit = ccl::as<const LogicalUnit *>(items[0].get());

    REQUIRE(logical_unit != nullptr);

    const auto *lhs_item = ccl::as<const Union *>(logical_unit->getLhs().get());
    const auto *rhs_item = ccl::as<const Union *>(logical_unit->getRhs().get());

    REQUIRE(lhs_item != nullptr);
    REQUIRE(rhs_item != nullptr);
}

TEST_CASE("LogicalItemWith3Elements", "[LogicalItem]")
{
    auto text_iterator = TextIterator{"([1] | [2]) | [3]"};
    auto special_items = lex::SpecialItems{};

    auto container = Container(text_iterator, special_items, 2);
    const auto &items = container.getItems();
    const auto *logical_unit = ccl::as<const LogicalUnit *>(items[0].get());

    REQUIRE(logical_unit != nullptr);

    const auto *lhs_item = ccl::as<const Container *>(logical_unit->getLhs().get());
    const auto *rhs_item = ccl::as<const Union *>(logical_unit->getRhs().get());

    REQUIRE(lhs_item != nullptr);
    REQUIRE(rhs_item != nullptr);
}