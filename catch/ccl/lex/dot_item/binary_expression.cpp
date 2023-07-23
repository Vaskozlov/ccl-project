#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace text;
using namespace lex::dot_item;

TEST_CASE("BinaryExpressionWith2Elements", "[BinaryExpression]")
{
    auto text_iterator = TextIterator{"[1] | \"2\""};
    auto special_items = lex::SpecialItems{};

    auto container = Container(text_iterator, special_items, 2);
    const auto &items = container.getItems();
    const auto *binary_operation = ccl::as<const BinaryExpression *>(items[0].get());

    REQUIRE(binary_operation != nullptr);

    const auto *lhs_item = ccl::as<const Union *>(binary_operation->getLhs().get());
    const auto *rhs_item = ccl::as<const Union *>(binary_operation->getRhs().get());

    REQUIRE(lhs_item != nullptr);
    REQUIRE(rhs_item != nullptr);
}

TEST_CASE("BinaryExpressionWith3Elements", "[BinaryExpression]")
{
    auto text_iterator = TextIterator{"[1] | [2] | [3]"};
    auto special_items = lex::SpecialItems{};

    auto container = Container(text_iterator, special_items, 2);
    const auto &items = container.getItems();
    const auto *first_binary_operation = ccl::as<const BinaryExpression *>(items[0].get());

    REQUIRE(first_binary_operation != nullptr);

    const auto *second_binary_operation =
        ccl::as<const BinaryExpression *>(first_binary_operation->getLhs().get());
    const auto *rhs_item = ccl::as<const Union *>(first_binary_operation->getRhs().get());

    REQUIRE(second_binary_operation != nullptr);
    REQUIRE(rhs_item != nullptr);
}