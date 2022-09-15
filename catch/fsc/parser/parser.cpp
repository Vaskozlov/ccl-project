#include <ccl/debug/debug_file.hpp>
#include <fsc/parser/parser.hpp>

using namespace ccl;
using namespace fsc;

BOOST_AUTO_TEST_CASE(FscParser)
{
    auto tokenizer = FscLexer.getTokenizer("func test() -> int\n");
    [[maybe_unused]] auto parser = Parser(tokenizer);
}
