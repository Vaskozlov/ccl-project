#include <ccl/handler/cmd_handler.hpp>
#include <fsc/lex/lexical_analyzer.hpp>

namespace fsc
{
    // NOLINTNEXTLINE
    ccl::lex::LexicalAnalyzer FscLexer(
        ccl::handler::Cmd::instance(),
        {
            { FscLexerToken::NEW_LINE, R"( ! "\n" )" },
            { FscLexerToken::COMMENT, R"( ! "//"[\n]*^ )" },
            { FscLexerToken::COMMENT, R"( ! "/*" "*/"^* "*/" )" },
            { FscLexerToken::FUNC, R"( "func" )" },
            { FscLexerToken::VAR, R"( "var" )" },
            { FscLexerToken::CLASS, R"( "class" )" },
            { FscLexerToken::ARROW, R"( "->" )" },
            { FscLexerToken::LEFT_PARENTHESIS, R"( ! [(] )" },
            { FscLexerToken::RIGHT_PARENTHESIS, R"( ! [)] )" },
            { FscLexerToken::ANGLE_OPENING, R"( ! "[" )" },
            { FscLexerToken::ANGLE_CLOSING, R"( ! "]" )" },
            { FscLexerToken::COMMA, R"( ! [,] )" },
            { FscLexerToken::COLON, R"( ! [:] )" },
            { FscLexerToken::ADDITION, R"( ! "+" )" },
            { FscLexerToken::SUBTRACTION, R"( ! "-" )" },
            { FscLexerToken::MULTIPLICATION, R"( ! "*" )" },
            { FscLexerToken::DIVISION, R"( ! "/" )" },
            { FscLexerToken::ASSIGNMENT, R"( ! "=" )" },
            { FscLexerToken::NUMBER, R"( ! [0-9]+ )" },
            { FscLexerToken::IDENTIFIER, R"( [a-zA-Z_][a-zA-Z0-9_]* )" },
        });

    const std::unordered_map<size_t, ccl::string_view> ToStringFscLexerToken
{
        { FscLexerToken::EOI, "EOI" },
        { FscLexerToken::BAD_TOKEN, "BAD_TOKEN" },
        { FscLexerToken::NEW_LINE, "NEW_LINE" },
        { FscLexerToken::COMMENT, "COMMENT" },
        { FscLexerToken::FUNC, "FUNC" },
        { FscLexerToken::VAR, "VAR" },
        { FscLexerToken::CLASS, "CLASS" },
        { FscLexerToken::ARROW, "ARROW" },
        { FscLexerToken::LEFT_PARENTHESIS, "LEFT_PARENTHESIS" },
        { FscLexerToken::RIGHT_PARENTHESIS, "RIGHT_PARENTHESIS" },
        { FscLexerToken::ANGLE_OPENING, "ANGLE_OPENING" },
        { FscLexerToken::ANGLE_CLOSING, "ANGLE_CLOSING" },
        { FscLexerToken::COMMA, "COMMA" },
        { FscLexerToken::COLON, "COLON" },
        { FscLexerToken::ADDITION, "ADDITION" },
        { FscLexerToken::SUBTRACTION, "SUBTRACTION" },
        { FscLexerToken::MULTIPLICATION, "MULTIPLICATION" },
        { FscLexerToken::DIVISION, "DIVISION" },
        { FscLexerToken::ASSIGNMENT, "ASSIGNMENT" },
        { FscLexerToken::NUMBER, "NUMBER" },
        { FscLexerToken::IDENTIFIER, "IDENTIFIER" },
    };
}// namespace fsc
