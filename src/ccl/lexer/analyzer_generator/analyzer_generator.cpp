#include <ccl/lexer/analyzer_generator/analyzer_generator.hpp>
#include <ccl/lexer/analyzer_generator/static_generator.hpp>
#include <ccll/ccll.hpp>
#include <isl/io.hpp>

namespace ccl::lexer
{
    auto AnalyzerGenerator::generateStaticVersion(const std::filesystem::path &path) -> std::string
    {
        const auto filename = path.string();
        const auto file_content = isl::io::read(path);

        return generateStaticVersion(filename, file_content);
    }

    auto AnalyzerGenerator::generateStaticVersion(std::string_view filename, isl::string_view text)
        -> std::string
    {
        auto tokenizer = ccll::CcllAnalyzer.getTokenizer(text, filename);
        const auto static_generator = gen::StaticGenerator{tokenizer};

        return static_generator.getCode();
    }
}// namespace ccl::lexer
