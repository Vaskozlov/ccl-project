#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/analyzer_generator/analyzer_generator.hpp>
#include <ccl/lexer/analyzer_generator/static_generator.hpp>
#include <ccll/ccll.hpp>
#include <fstream>
#include <sstream>

namespace ccl::lexer
{
    static auto readFile(const std::filesystem::path &path) -> std::string
    {
        auto stream = std::ifstream(path);

        if (!stream.is_open()) {
            fmt::print("Error: cannot open file {}\n", path.string());
            std::cout.flush();
            throw std::invalid_argument("Failed to open file");
        }

        auto buffer = std::stringstream{};
        buffer << stream.rdbuf();

        return buffer.str();
    }

    auto AnalyzerGenerator::generateStaticVersion(const std::filesystem::path &path) -> std::string
    {
        auto filename = path.string();
        auto file_content = readFile(path);

        return generateStaticVersion(filename, file_content);
    }

    auto AnalyzerGenerator::generateStaticVersion(std::string_view filename, string_view text)
        -> std::string
    {
        auto tokenizer = ccll::CcllAnalyzer.getTokenizer(text, filename);
        auto static_generator = gen::StaticGenerator{tokenizer};

        return static_generator.getCode();
    }
}// namespace ccl::lexer
