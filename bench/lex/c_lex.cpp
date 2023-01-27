#include <benchmark/benchmark.h>
#include <chrono>

#include "c_lexer.hpp"

#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/tokenizer.hpp>

using namespace std::string_view_literals;

const auto CProgram = R"(
int main(int argc, char **argv) {
    int a = 10;
    int b = 20;
    char *c = "Hello, World!\xff";
    char d = 't';

      /*
    This is a comment!*/

    float a2 = 20.0f;

    a += b;
    b *= a;

    return a + b;
}

int main(int argc, char **argv) {
    int a = 10;
    int b = 20;
    char *c = "Hello, World!\xff";
    char d = 't';

      /*
    This is a comment!*/

    float a2 = 20.0f;

    a += b;
    b *= a;

    return a + b;
}

int main(int argc, char **argv) {
    int a = 10;
    int b = 20;
    char *c = "Hello, World!\xff";
    char d = 't';

      /*
    This is a comment!*/

    float a2 = 20.0f;

    a += b;
    b *= a;

    return a + b;
}

int main(int argc, char **argv) {
    int a = 10;
    int b = 20;
    char *c = "Hello, World!\xff";
    char d = 't';

      /*
    This is a comment!*/

    float a2 = 20.0f;

    a += b;
    b *= a;

    return a + b;
}
)"sv;

static auto lexicalAnalysisOfC(benchmark::State &state) -> void
{
    for (auto _ : state) {
        auto tokenizer = CLexer.getTokenizer(CProgram);

        while (const auto &token = tokenizer.yield()) {
            benchmark::DoNotOptimize(token);
            benchmark::DoNotOptimize(token.getRepr());
            benchmark::DoNotOptimize(token.getLine());
            benchmark::DoNotOptimize(token.getColumn());
        }
    }
}

BENCHMARK(lexicalAnalysisOfC);

static auto lexicalAnalyzerInitialization(benchmark::State &state) -> void
{
    for (auto _ : state) {
        [[maybe_unused]] auto c_lexer = ccl::lex::LexicalAnalyzer(
            ccl::handler::Cmd::instance(),
            {
                {CLexerToken::COMMENT, R"( ! "//"[\n]*^ )"},
                {CLexerToken::COMMENT, R"( ! "/*" "*/"^* "*/" )"},
                {CLexerToken::CURLY_OPENING, R"( ! [{] )"},
                {CLexerToken::CURLY_CLOSING, R"( ! [}] )"},
                {CLexerToken::LEFT_PARENTHESIS, R"( ! [(] )"},
                {CLexerToken::RIGHT_PARENTHESIS, R"( ! [)] )"},
                {CLexerToken::ANGLE_OPENING, R"( ! [\[] )"},
                {CLexerToken::ANGLE_CLOSING, R"( ! [\]] )"},
                {CLexerToken::COMMA, R"( ! [,] )"},
                {CLexerToken::COLUMN, R"( ! [;] )"},
                {CLexerToken::PREPROCESSOR, R"( ! [#] )"},
                {CLexerToken::QUESTION_MARK, R"( ! [?] )"},
                {CLexerToken::ADD, R"( ! [+] )"},
                {CLexerToken::SUB, R"( ! "-" )"},
                {CLexerToken::MUL, R"( ! [*] )"},
                {CLexerToken::DIV, R"( ! [/] )"},
                {CLexerToken::MOD, R"( ! [%] )"},
                {CLexerToken::ASSIGN, R"( ! [=] )"},
                {CLexerToken::ADD_ASSIGN, R"( ! "+=" )"},
                {CLexerToken::SUB_ASSIGN, R"( ! "-=" )"},
                {CLexerToken::MUL_ASSIGN, R"( ! "*=" )"},
                {CLexerToken::DIB_ASSIGN, R"( ! "/=" )"},
                {CLexerToken::MOD_ASSIGN, R"( ! "%=" )"},
                {CLexerToken::OR, R"( ! [|] )"},
                {CLexerToken::AND, R"( ! [&] )"},
                {CLexerToken::XOR, R"( ! [^] )"},
                {CLexerToken::BITWISE_NOT, R"( ! [~] )"},
                {CLexerToken::OR_ASSIGN, R"( ! "|=" )"},
                {CLexerToken::AND_ASSIGN, R"( ! "&=" )"},
                {CLexerToken::XOR_ASSIGN, R"( ! "^=" )"},
                {CLexerToken::LESS, R"( ! "<" )"},
                {CLexerToken::GREATER, R"( ! ">" )"},
                {CLexerToken::EQUAL, R"( ! "==" )"},
                {CLexerToken::NOT_EQUAL, R"( ! "!=" )"},
                {CLexerToken::LESS_EQUAL, R"( ! "<=" )"},
                {CLexerToken::GREATER_EQUAL, R"( ! ">=" )"},
                {CLexerToken::LOGICAL_OR, R"( ! "||" )"},
                {CLexerToken::LOGICAL_AND, R"( ! "&&" )"},
                {CLexerToken::LOGICAL_NOT, R"( ! [!] )"},
                {CLexerToken::FOR, R"( "for" )"},
                {CLexerToken::WHILE, R"( "while" )"},
                {CLexerToken::SWITCH, R"( "switch" )"},
                {CLexerToken::CASE, R"( "case" )"},
                {CLexerToken::RETURN, R"( "return" )"},
                {CLexerToken::STRUCT, R"( "struct" )"},
                {CLexerToken::NUMBER, R"( [0-9]+ ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p )"},
                {CLexerToken::FLOAT, R"( [0-9]+"."[0-9]* ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p )"},
                {CLexerToken::IDENTIFIER, R"( [a-zA-Z_]+[a-zA-Z0-9_]* )"},
                {CLexerToken::CHAR, R"( ! "\'" ([']^ | "\\\'" ) "\'" )"},
                {CLexerToken::STRING, R"( ! "\"" (["]^ | "\\\"")* "\"" )"},
            });
    }
}

BENCHMARK(lexicalAnalyzerInitialization);
