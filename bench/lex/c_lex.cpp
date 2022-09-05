#include <benchmark/benchmark.h>
#include <chrono>

#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

CCL_PREDEFINED_ENUM(
    Token_C_Lexer,
    size_t,
    COMMENT = 2,
    CURLY_OPENING = 65536,
    CURLY_CLOSING = 65537,
    LEFT_PARENTHESIS = 65538,
    RIGHT_PARENTHESIS = 65539,
    ANGLE_OPENING = 65540,
    ANGLE_CLOSING = 65541,
    COMMA = 65542,
    SEPARATOR = 65543,
    PREPROCESSOR = 65544,
    QUESTION_MARK = 65545,
    ADD = 131072,
    SUB = 131073,
    MUL = 131074,
    DIV = 131075,
    MOD = 131076,
    ASSIGN = 131077,
    ADD_ASSIGN = 131078,
    SUB_ASSIGN = 131079,
    MUL_ASSIGN = 131080,
    DIB_ASSIGN = 131081,
    MOD_ASSIGN = 131082,
    OR = 131083,
    AND = 131084,
    XOR = 131085,
    BITWISE_NOT = 131086,
    OR_ASSIGN = 131087,
    AND_ASSIGN = 131088,
    XOR_ASSIGN = 131089,
    LESS = 131090,
    GREATER = 131091,
    EQUAL = 131092,
    NOT_EQUAL = 131093,
    LESS_EQUAL = 131094,
    GREATER_EQUAL = 131095,
    LOGICAL_OR = 131096,
    LOGICAL_AND = 131097,
    LOGICAL_NOT = 131098,
    FOR = 196608,
    WHILE = 196609,
    SWITCH = 196610,
    CASE = 196611,
    RETURN = 196612,
    STRUCT = 196613,
    INT_T = 262144,
    CHAR_T = 262145,
    LONG_T = 262146,
    SHORT_T = 262147,
    FLOAT_T = 262148,
    DOUBLE_T = 262149,
    SIGNED_T = 262150,
    UNSIGNED_T = 262151,
    NUMBER = 327680,
    FLOAT = 327681,
    IDENTIFIER = 327682,
    CHAR = 327683,
    STRING = 327684);

static inline ccl::lex::LexicalAnalyzer C_Lexer(
    ccl::handler::Cmd::instance(),
    {
        { Token_C_Lexer::COMMENT, R"( ! "//"[\n]*^ )" },
        { Token_C_Lexer::COMMENT, R"( ! "/*" "*/"^* "*/" )" },
        { Token_C_Lexer::CURLY_OPENING, R"( ! "{" )" },
        { Token_C_Lexer::CURLY_CLOSING, R"( ! "}" )" },
        { Token_C_Lexer::LEFT_PARENTHESIS, R"( ! "(" )" },
        { Token_C_Lexer::RIGHT_PARENTHESIS, R"( ! [)] )" },
        { Token_C_Lexer::ANGLE_OPENING, R"( ! "[" )" },
        { Token_C_Lexer::ANGLE_CLOSING, R"( ! "]" )" },
        { Token_C_Lexer::COMMA, R"( ! "," )" },
        { Token_C_Lexer::SEPARATOR, R"( ! ";" )" },
        { Token_C_Lexer::PREPROCESSOR, R"( ! "#" )" },
        { Token_C_Lexer::QUESTION_MARK, R"( ! "?" )" },
        { Token_C_Lexer::ADD, R"( ! "+" )" },
        { Token_C_Lexer::SUB, R"( ! "-" )" },
        { Token_C_Lexer::MUL, R"( ! "*" )" },
        { Token_C_Lexer::DIV, R"( ! "/" )" },
        { Token_C_Lexer::MOD, R"( ! "%" )" },
        { Token_C_Lexer::ASSIGN, R"( ! "=" )" },
        { Token_C_Lexer::ADD_ASSIGN, R"( ! "+=" )" },
        { Token_C_Lexer::SUB_ASSIGN, R"( ! "-=" )" },
        { Token_C_Lexer::MUL_ASSIGN, R"( ! "*=" )" },
        { Token_C_Lexer::DIB_ASSIGN, R"( ! "/=" )" },
        { Token_C_Lexer::MOD_ASSIGN, R"( ! "%=" )" },
        { Token_C_Lexer::OR, R"( ! "|" )" },
        { Token_C_Lexer::AND, R"( ! "&" )" },
        { Token_C_Lexer::XOR, R"( ! "^" )" },
        { Token_C_Lexer::BITWISE_NOT, R"( ! "~" )" },
        { Token_C_Lexer::OR_ASSIGN, R"( ! "|=" )" },
        { Token_C_Lexer::AND_ASSIGN, R"( ! "&=" )" },
        { Token_C_Lexer::XOR_ASSIGN, R"( ! "^=" )" },
        { Token_C_Lexer::LESS, R"( ! "<" )" },
        { Token_C_Lexer::GREATER, R"( ! ">" )" },
        { Token_C_Lexer::EQUAL, R"( ! "==" )" },
        { Token_C_Lexer::NOT_EQUAL, R"( ! "!=" )" },
        { Token_C_Lexer::LESS_EQUAL, R"( ! "<=" )" },
        { Token_C_Lexer::GREATER_EQUAL, R"( ! ">=" )" },
        { Token_C_Lexer::LOGICAL_OR, R"( ! "||" )" },
        { Token_C_Lexer::LOGICAL_AND, R"( ! "&&" )" },
        { Token_C_Lexer::LOGICAL_NOT, R"( ! "!" )" },
        { Token_C_Lexer::FOR, R"( "for" )" },
        { Token_C_Lexer::WHILE, R"( "while" )" },
        { Token_C_Lexer::SWITCH, R"( "switch" )" },
        { Token_C_Lexer::CASE, R"( "case" )" },
        { Token_C_Lexer::RETURN, R"( "return" )" },
        { Token_C_Lexer::STRUCT, R"( "struct" )" },
        { Token_C_Lexer::INT_T, R"( "int" )" },
        { Token_C_Lexer::CHAR_T, R"( "char" )" },
        { Token_C_Lexer::LONG_T, R"( "long" )" },
        { Token_C_Lexer::SHORT_T, R"( "short" )" },
        { Token_C_Lexer::FLOAT_T, R"( "float" )" },
        { Token_C_Lexer::DOUBLE_T, R"( "double" )" },
        { Token_C_Lexer::SIGNED_T, R"( "signed" )" },
        { Token_C_Lexer::UNSIGNED_T, R"( "unsigned" )" },
        { Token_C_Lexer::NUMBER, R"( [0-9]+ ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p )" },
        { Token_C_Lexer::FLOAT, R"( [0-9]+"."[0-9]* ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p )" },
        { Token_C_Lexer::IDENTIFIER, R"( [a-zA-Z_]+[a-zA-Z0-9_]* )" },
        { Token_C_Lexer::CHAR, R"( ! "\'" ([']^ | "\\\'" ) "\'" )" },
        { Token_C_Lexer::STRING, R"( ! "\"" (["]^ | "\\\"")* "\"" )" },
    });

auto C_Program = R"(
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
)";

static void lexicalAnalysisOfC()
{
    auto begin = std::chrono::high_resolution_clock::now();

    auto tokenizer = C_Lexer.getTokenizer(C_Program);
    auto token = tokenizer.yield();

    while (token) {
        fmt::print("{}\n", token.getRepr());
        token = tokenizer.yield();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - begin;
    fmt::print("{:e}\n", elapsed.count());
}

auto call = []() {
    lexicalAnalysisOfC();
    return true;
}();
