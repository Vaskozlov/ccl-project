#include "peg_grammar.hpp"
#include <ccl/debug/debug_file.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/peg_parser.hpp>

TEST_CASE("PegParsing", "[Parser]")
{
    using namespace isl;
    using namespace ccl;

    auto input = string_view{"10+2*3"};
    auto tokenizer = tparser::TestParser.getParser("EXPR", input, "stdin");

    tokenizer.parse()->print("", false, [](Id value) {
        return lexer::lexerEnumToString<tparser::TestParserToken>(
            as<tparser::TestParserToken>(value));
    });
}
