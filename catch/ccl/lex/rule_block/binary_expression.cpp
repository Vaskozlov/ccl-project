#include <ccl/debug/debug.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/lexer/rule/container.hpp>
#include <ccl/lexer/rule/sequence.hpp>
#include <ccl/lexer/rule/union.hpp>

TEST_CASE("BinaryExpressionWith2Elements", "[BinaryExpression]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"[1] | \"2\""};
    auto empty_lexical_analyzer =
        ccl::lexer::LexicalAnalyzer(ccl::ExceptionHandler::instance(), {});

    auto container =
        lexer::rule::Container(empty_lexical_analyzer, text_iterator, debug::FirstUsableToken);

    const std::vector<lexer::rule::RuleBlock> &items = container.getItems();

    const auto *binary_operation =
        isl::as<const lexer::rule::BinaryExpressionBase *>(items.at(0).get());

    REQUIRE(isl::is<lexer::rule::BinaryExpressionBase *>(items.at(0).get()));
    REQUIRE(isl::is<lexer::rule::Union *>(binary_operation->getLhs().get()));
    REQUIRE(isl::is<lexer::rule::Sequence *>(binary_operation->getRhs().get()));
}

TEST_CASE("BinaryExpressionWith3Elements", "[BinaryExpression]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"[1] | [2] | [3]"};
    auto empty_lexical_analyzer = lexer::LexicalAnalyzer(ExceptionHandler::instance(), {});

    auto container =
        lexer::rule::Container(empty_lexical_analyzer, text_iterator, debug::FirstUsableToken);

    const std::vector<lexer::rule::RuleBlock> &items = container.getItems();

    const auto *first_binary_operation =
        isl::as<const lexer::rule::BinaryExpressionBase *>(items.at(0).get());

    REQUIRE(first_binary_operation != nullptr);
    REQUIRE(
        isl::is<const lexer::rule::BinaryExpressionBase *>(first_binary_operation->getLhs().get()));
    REQUIRE(isl::is<const lexer::rule::Union *>(first_binary_operation->getRhs().get()));
}