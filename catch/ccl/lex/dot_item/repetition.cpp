#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

using namespace ccl;
using namespace text;
using namespace lexer::dot_item;

TEST_CASE("RepetitionBasicCase", "[ContainerRepetition]")
{
    auto text_iterator = TextIterator{"{10, 20}"};
    text_iterator.advance();

    DEBUG_VAR repetition = Repetition{text_iterator};
    REQUIRE(repetition.from == 10);
    REQUIRE(repetition.to == 20);
}

TEST_CASE("RepetitionEmptyFirstArgument", "[ContainerRepetition]")
{
    auto text_iterator = TextIterator{"{, 1}"};
    text_iterator.advance();

    DEBUG_VAR repetition = Repetition{text_iterator};

    REQUIRE(repetition.from == 0);
    REQUIRE(repetition.to == 1);
}

TEST_CASE("RepetitionFirstArgumentGreaterThanSecond", "[ContainerRepetition]")
{
    auto text_iterator = TextIterator{"{2, 1}"};
    text_iterator.advance();

    REQUIRE_THROWS_AS(Repetition{text_iterator}, text::TextIteratorException);
}
