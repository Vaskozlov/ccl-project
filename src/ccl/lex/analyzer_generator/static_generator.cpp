#include <ccl/lex/analyzer_generator/ccll_parser.hpp>
#include <ccl/lex/analyzer_generator/static_generator.hpp>
#include <fmt/format.h>

namespace ccl::lex::gen
{
    auto StaticGenerator::generateHeader() -> void
    {
        auto parser = parser::CcllParser{ tokenizer };
        parser.parse();
    }
}// namespace ccl::lex::gen
