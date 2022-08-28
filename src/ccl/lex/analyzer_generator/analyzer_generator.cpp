#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <ccl/lex/analyzer_generator/static_generator.hpp>
#include <fstream>
#include <sstream>

namespace ccl::lex
{
    // NOLINTNEXTLINE global variable
    auto AnalyzerGenerator::LexForGenerator = LexicalAnalyzer{
        { GenToken::IDENTIFIER, u8"[a-zA-Z_]+[a-zA-Z0-9_]*" },
        { GenToken::INTEGER, u8"[0-9]+" },
        { GenToken::RAW_DATA, u8R"([\t\n=[[ ]^+[\n]*^)" },
        { GenToken::NEW_LINE, u8R"('\n')" },
        { GenToken::COLUMN, u8R"(':')" },
        { GenToken::ASSIGN, u8R"('=')" },
        { GenToken::ANGLE_OPENING, u8R"('[')" },
        { GenToken::ANGLE_CLOSING, u8R"(']')" },
    };

    static auto readFile(const std::filesystem::path &path) -> std::string
    {
        auto stream = std::ifstream(path);

        if (!stream.is_open()) {
            throw InvalidArgument("Failed to open file: " + path.string());
        }

        auto buffer = std::stringstream{};
        buffer << stream.rdbuf();

        return buffer.str();
    }

    auto AnalyzerGenerator::generateStaticVersion(const std::filesystem::path &path) -> void
    {
        auto filename = path.filename().string();
        auto casted_filename =// NOLINTNEXTLINE reinterpret_cast
            u8string_view{ reinterpret_cast<const char8_t *>(filename.c_str()), filename.size() };

        auto file_content = readFile(path);
        auto casted_file_content =// NOLINTNEXTLINE reinterpret_cast
            u8string_view{ reinterpret_cast<const char8_t *>(file_content.data()),
                           file_content.size() };

        generateStaticVersion(casted_file_content, casted_filename);
    }

    auto AnalyzerGenerator::generateStaticVersion(u8string_view text, u8string_view filename)
        -> void
    {
        auto tokenizer = LexForGenerator.getTokenizer(text, filename);
        auto static_generator = gen::StaticGenerator{ tokenizer };
    }
}// namespace ccl::lex
