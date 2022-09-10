#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <ccl/lex/analyzer_generator/static_generator.hpp>
#include <ccl/lex/lexical_analyzer.hpp>
#include <fstream>
#include <sstream>

namespace ccl::lex
{
    // NOLINTNEXTLINE global variable
    LexicalAnalyzer AnalyzerGenerator::LexForGenerator{
        handler::Cmd::instance(),
        { { GenToken::IDENTIFIER, "[a-zA-Z_]+[a-zA-Z0-9_]*" },
          { GenToken::INTEGER, "[0-9]+" },
          { GenToken::RULE_DECLARATION, R"(["!'([]+[\n]*^)" },
          { GenToken::STRING, R"( "'" ( "\'"^ | "\\\'" )* "\'" )" },
          { GenToken::NEW_LINE, R"(! "\n")" },
          { GenToken::COLUMN, R"(! ":")" },
          { GenToken::ASSIGN, R"(! "=")" },
          { GenToken::CURLY_OPENING, R"(! "{")" },
          { GenToken::CURLY_CLOSING, R"(! "}")" } }
    };

    static auto readFile(const std::filesystem::path &path) -> std::string
    {
        auto stream = std::ifstream(path);

        if (!stream.is_open()) {
            fmt::print("Error: cannot open file {}\n", path.string());
            std::cout.flush();
            throw InvalidArgument("Failed to open file");
        }

        auto buffer = std::stringstream{};
        buffer << stream.rdbuf();

        return buffer.str();
    }

    auto AnalyzerGenerator::generateStaticVersion(const std::filesystem::path &path) -> void
    {
        auto filename = path.string();
        auto file_content = readFile(path);

        generateStaticVersion(filename, file_content);
    }

    auto AnalyzerGenerator::generateStaticVersion(string_view filename, string_view text) -> void
    {
        auto tokenizer = LexForGenerator.getTokenizer(text, filename);
        auto static_generator = gen::StaticGenerator{ tokenizer };
    }
}// namespace ccl::lex
