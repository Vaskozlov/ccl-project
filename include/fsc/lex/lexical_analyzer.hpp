#pragma once

#include <ccl/lex/lexical_analyzer.hpp>
#include <unordered_map>

namespace fsc
{
    CCL_PREDEFINED_ENUM(// NOLINTNEXTLINE
        FscLexerToken,
        size_t,
        BINARY_OPERATOR = 196608,
        KEYWORD = 65536,
        NONE = 0,
        SEPARATORS = 131072,
        VALUE = 262144,
        EOI = 0,
        BAD_TOKEN = 1,
        NEW_LINE = 2,
        COMMENT = 3,
        FUNC = 65536,
        VAR = 65537,
        CLASS = 65538,
        ARROW = 65539,
        LEFT_PARENTHESIS = 131072,
        RIGHT_PARENTHESIS = 131073,
        ANGLE_OPENING = 131074,
        ANGLE_CLOSING = 131075,
        COMMA = 131076,
        COLON = 131077,
        ADDITION = 196608,
        SUBTRACTION = 196609,
        MULTIPLICATION = 196610,
        DIVISION = 196611,
        ASSIGNMENT = 196612,
        NUMBER = 262144,
        IDENTIFIER = 262145);

    extern ccl::lex::LexicalAnalyzer FscLexer;
    extern const std::unordered_map<size_t, ccl::string_view> ToStringFscLexerToken;
}// namespace fsc
