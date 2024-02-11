#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

TEST_CASE("BinaryExpressionWith2Elements", "[BinaryExpression]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"[1] | \"2\""};
    auto special_items = lexer::SpecialItems{};

    auto container =
        lexer::dot_item::Container(text_iterator, special_items, debug::FirstUsableToken);

    const std::vector<lexer::dot_item::DotItem> &items = container.getItems();

    const auto *binary_operation =
        isl::as<const lexer::dot_item::BinaryExpressionBase *>(items[0].get());

    REQUIRE(binary_operation != nullptr);

    const auto *lhs_item =
        isl::as<const lexer::dot_item::Union *>(binary_operation->getLhs().get());

    const auto *rhs_item =
        isl::as<const lexer::dot_item::Sequence *>(binary_operation->getRhs().get());

    REQUIRE(lhs_item != nullptr);
    REQUIRE(rhs_item != nullptr);
}

TEST_CASE("BinaryExpressionWith3Elements", "[BinaryExpression]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"[1] | [2] | [3]"};
    auto special_items = lexer::SpecialItems{};

    auto container =
        lexer::dot_item::Container(text_iterator, special_items, debug::FirstUsableToken);

    const std::vector<lexer::dot_item::DotItem> &items = container.getItems();

    const auto *first_binary_operation =
        isl::as<const lexer::dot_item::BinaryExpressionBase *>(items[0].get());

    REQUIRE(first_binary_operation != nullptr);

    const auto *second_binary_operation = isl::as<const lexer::dot_item::BinaryExpressionBase *>(
        first_binary_operation->getLhs().get());

    const auto *rhs_item =
        isl::as<const lexer::dot_item::Union *>(first_binary_operation->getRhs().get());

    REQUIRE(second_binary_operation != nullptr);
    REQUIRE(rhs_item != nullptr);
}