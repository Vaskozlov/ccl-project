#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <ccl/lex/analyzer_generator/static_generator.hpp>
#include <ccl/lex/tokenizer.hpp>
#include <fstream>
#include <sstream>

namespace ccl::lex
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

    auto AnalyzerGenerator::generateStaticVersion(string_view filename, string_view text)
        -> std::string
    {
        static LexicalAnalyzer lex_for_generator{
            handler::Cmd::instance(),
            {{GenToken::DIRECTIVE,
              R"( ( [a-zA-Z_][a-zA-Z_0-9]* )p [ \t]* [=] [ \t]* ("\"" (["]^ | "\\\"" )* "\"")p )"},
             {GenToken::BAD_DIRECTIVE_DECLARATION, R"( ( [a-zA-Z_][a-zA-Z_0-9]* )p [ \t]* [=] )"},
             {GenToken::RULE, R"( ( [a-zA-Z_][a-zA-Z_0-9]* )p [ \t]* [:] ( [\n]+^ )p)"},
             {GenToken::BAD_RULE_OR_DIRECTIVE_DECLARATION, R"( [a-zA-Z_][a-zA-Z_0-9]* )"},
             {GenToken::BAD_RULE_DECLARATION, R"( ( [a-zA-Z_][a-zA-Z_0-9]* )p [ \t]* [:] [ \t]* )"},
             {GenToken::GROUP_DECLARATION, R"( [\[]p ([a-zA-Z_][a-zA-Z_0-9]*)p [\]]p )"},
             {GenToken::BAD_GROUP_DECLARATION_ONLY_BRACKET, R"( [\[] )"},
             {GenToken::BAD_GROUP_DECLARATION_BRACKET_AND_NAME, R"( [\[] [a-zA-Z_][a-zA-Z_0-9]*)"},
             {GenToken::BAD_GROUP_DECLARATION_EMPTY_NAME, R"( [\[] [\]])"},
             {GenToken::BAD_GROUP_NO_OPEN_BRACKET, R"( [a-zA-Z_][a-zA-Z_0-9]* [\]])"}}};

        auto tokenizer = lex_for_generator.getTokenizer(text, filename);
        auto static_generator = gen::StaticGenerator{tokenizer};

        return static_generator.getCode();
    }
}// namespace ccl::lex
