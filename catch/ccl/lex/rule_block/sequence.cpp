#include <ccl/debug/debug.hpp>
#include <ccl/lexer/rule/sequence.hpp>

TEST_CASE("SequenceWithOneCharBegin", "[ContainerSequence]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{R"("Hello, \"World\"!")"};
    text_iterator.advance();

    auto string_item = lexer::rule::Sequence({}, "\"", text_iterator);
    DEBUG_VAR string = string_item.getValue();

    REQUIRE(string == R"(Hello, "World"!)");
    REQUIRE(ccl::isEoF(text_iterator.advance()));
}

TEST_CASE("SequenceWithTreeCharBegin", "[ContainerSequence]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"\"\"\"Hello,\n    \"World\"!\"\"\""};
    text_iterator.advance();

    auto string_item = lexer::rule::Sequence({.multiline = true}, R"(""")", text_iterator);
    DEBUG_VAR string = string_item.getValue();

    REQUIRE(string == "Hello,\n    \"World\"!");
    REQUIRE(ccl::isEoF(text_iterator.advance()));
}

TEST_CASE("UnterminatedSequence", "[ContainerSequence]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{R"("Hello, World!)"};
    text_iterator.advance();

    CHECK_THROWS_AS(lexer::rule::Sequence({}, "\"", text_iterator), text::TextIteratorException);
}

TEST_CASE("SequenceReachedNewLine", "[ContainerSequence]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"\"Hello, World!\n\""};
    text_iterator.advance();

    CHECK_THROWS_AS(lexer::rule::Sequence({}, "\"", text_iterator), text::TextIteratorException);
}
