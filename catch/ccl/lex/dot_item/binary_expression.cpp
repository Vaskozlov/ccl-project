#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>
#include <ccl/lexer/dot_item/sequence.hpp>
#include <ccl/lexer/dot_item/union.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>

TEST_CASE("BinaryExpressionWith2Elements", "[BinaryExpression]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"[1] | \"2\""};
    auto special_items = lexer::AnyPlaceItems{};
    auto empty_lexical_analyzer =
        ccl::lexer::LexicalAnalyzer(ccl::ExceptionHandler::instance(), {});

    auto container = lexer::dot_item::Container(
        empty_lexical_analyzer, text_iterator, special_items, debug::FirstUsableToken);

    const std::vector<lexer::dot_item::DotItem> &items = container.getItems();

    const auto *binary_operation =
        isl::as<const lexer::dot_item::BinaryExpressionBase *>(items[0].get());

    REQUIRE(isl::is<lexer::dot_item::BinaryExpressionBase *>(items[0].get()));
    REQUIRE(isl::is<lexer::dot_item::Union *>(binary_operation->getLhs().get()));
    REQUIRE(isl::is<lexer::dot_item::Sequence *>(binary_operation->getRhs().get()));
}

TEST_CASE("BinaryExpressionWith3Elements", "[BinaryExpression]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"[1] | [2] | [3]"};
    auto special_items = lexer::AnyPlaceItems{};
    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});

    auto container = lexer::dot_item::Container(
        empty_lexical_analyzer, text_iterator, special_items, debug::FirstUsableToken);

    const std::vector<lexer::dot_item::DotItem> &items = container.getItems();

    const auto *first_binary_operation =
        isl::as<const lexer::dot_item::BinaryExpressionBase *>(items[0].get());

    REQUIRE(first_binary_operation != nullptr);
    REQUIRE(isl::is<const lexer::dot_item::BinaryExpressionBase *>(
        first_binary_operation->getLhs().get()));
    REQUIRE(isl::is<const lexer::dot_item::Union *>(first_binary_operation->getRhs().get()));
}