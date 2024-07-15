#include "basic_math_grammar.hpp"
#include "lr_parser_for_math.hpp"
#include <ccl/debug/debug.hpp>

TEST_CASE("LrTwoNumbersAddition", "[LrParser]")
{
    using namespace ccl::debug;

    auto tokenizer = math::MathLexer.getTokenizer("10+20");
    auto result = MathParser.parse(tokenizer);

    REQUIRE(result != nullptr);

    result->print("", false, [](auto elem) {
        return ccl::lexer::lexerEnumToString(isl::as<GrammarSymbol>(elem));
    });
}
