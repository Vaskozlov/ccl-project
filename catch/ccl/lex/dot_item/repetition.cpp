#include <ccl/debug/debug_file.hpp>
import ccl.lexer;

TEST_CASE("RepetitionBasicCase", "[ContainerRepetition]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"{10, 20}"};
    text_iterator.advance();

    DEBUG_VAR repetition = lexer::dot_item::Repetition{text_iterator};
    REQUIRE(repetition.from == 10);
    REQUIRE(repetition.to == 20);
}

TEST_CASE("RepetitionEmptyFirstArgument", "[ContainerRepetition]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"{, 1}"};
    text_iterator.advance();

    DEBUG_VAR repetition = lexer::dot_item::Repetition{text_iterator};

    REQUIRE(repetition.from == 0);
    REQUIRE(repetition.to == 1);
}

TEST_CASE("RepetitionFirstArgumentGreaterThanSecond", "[ContainerRepetition]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"{2, 1}"};
    text_iterator.advance();

    REQUIRE_THROWS_AS(lexer::dot_item::Repetition{text_iterator}, text::TextIteratorException);
}
