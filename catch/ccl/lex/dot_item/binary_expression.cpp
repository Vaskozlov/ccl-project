#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

using namespace ccl;
using namespace text;
using namespace lexer::dot_item;

TEST_CASE("BinaryExpressionWith2Elements", "[BinaryExpression]")
{
    auto text_iterator = TextIterator{"[1] | \"2\""};
    auto special_items = lexer::SpecialItems{};

    auto container = Container(text_iterator, special_items, 2);
    const std::vector<DotItem> &items = container.getItems();
    const auto *binary_operation = isl::as<const BinaryExpressionBase *>(items[0].get());

    REQUIRE(binary_operation != nullptr);

    const auto *lhs_item = isl::as<const Union *>(binary_operation->getLhs().get());
    const auto *rhs_item = isl::as<const Sequence *>(binary_operation->getRhs().get());

    REQUIRE(lhs_item != nullptr);
    REQUIRE(rhs_item != nullptr);
}

TEST_CASE("BinaryExpressionWith3Elements", "[BinaryExpression]")
{
    auto text_iterator = TextIterator{"[1] | [2] | [3]"};
    auto special_items = lexer::SpecialItems{};

    auto container = Container(text_iterator, special_items, 2);
    const std::vector<DotItem> &items = container.getItems();
    const auto *first_binary_operation = isl::as<const BinaryExpressionBase *>(items[0].get());

    REQUIRE(first_binary_operation != nullptr);

    const auto *second_binary_operation =
        isl::as<const BinaryExpressionBase *>(first_binary_operation->getLhs().get());
    const auto *rhs_item = isl::as<const Union *>(first_binary_operation->getRhs().get());

    REQUIRE(second_binary_operation != nullptr);
    REQUIRE(rhs_item != nullptr);
}