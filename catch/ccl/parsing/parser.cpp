#include "grammer.hpp"
#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/value.hpp>
#include <ccl/parser/peg_parser.hpp>

TEST_CASE("Parsing", "[Parser]")
{
    using namespace isl;
    using namespace ccl;

    auto input = string_view{"(10 + 20 + 30) * 3"};
    auto tokenizer = tparser::TestParser.getParser("EXPR", input, "stdin");

    tokenizer.parse()->print("", false, [](Id value) {
        return lexer::lexerEnumToString<tparser::TestParserToken>(
            as<tparser::TestParserToken>(value));
    });
}
