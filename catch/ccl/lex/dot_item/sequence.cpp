#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>
#include <ccl/lex/dot_item/sequence.hpp>

using namespace ccl;
using namespace lexer;
using namespace text;
using namespace dot_item;

// NOLINTNEXTLINE

TEST_CASE("SequenceWithOneCharBegin", "[ContainerSequence]")
{
    auto text_iterator = TextIterator{R"("Hello, \"World\"!")"};
    text_iterator.advance();

    auto string_item = Sequence({}, "\"", text_iterator);
    DEBUG_VAR &&string = string_item.getValue();

    REQUIRE(string == R"(Hello, "World"!)");
    REQUIRE(ccl::isEoF(text_iterator.advance()));
}

TEST_CASE("SequenceWithTreeCharBegin", "[ContainerSequence]")
{
    auto text_iterator = TextIterator{"\"\"\"Hello,\n    \"World\"!\"\"\""};
    text_iterator.advance();

    auto string_item = Sequence({.multiline = true}, R"(""")", text_iterator);
    DEBUG_VAR &&string = string_item.getValue();

    REQUIRE(string == "Hello,\n    \"World\"!");
    REQUIRE(ccl::isEoF(text_iterator.advance()));
}

TEST_CASE("UnterminatedSequence", "[ContainerSequence]")
{
    auto text_iterator = TextIterator{R"("Hello, World!)"};
    text_iterator.advance();

    CHECK_THROWS_AS(Sequence({}, "\"", text_iterator), text::TextIteratorException);
}

TEST_CASE("SequenceReachedNewLine", "[ContainerSequence]")
{
    auto text_iterator = TextIterator{"\"Hello, World!\n\""};
    text_iterator.advance();

    CHECK_THROWS_AS(Sequence({}, "\"", text_iterator), text::TextIteratorException);
}
