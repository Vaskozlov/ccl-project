#include <ccl/lex/analyzer_generator/ccll_parser.hpp>
#include <ccl/lex/analyzer_generator/static_generator.hpp>
#include <fmt/format.h>

namespace ccl::lex::gen
{
    [[maybe_unused]] static auto convert(ccl::u8string_view str) -> std::string_view
    {
        return { reinterpret_cast<const char *>(str.begin()), str.size() };// NOLINT
    }

    auto StaticGenerator::generateHeader() -> void
    {
        auto parser = parser::CcllParser{ tokenizer };
        parser.parse();
    }
}// namespace ccl::lex::gen
