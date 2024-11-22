#include <ast-lang-2/ast-lang.hpp>

namespace astlang2
{
    auto getAstlangGrammar() -> isl::string_view
    {
        constexpr static isl::string_view ast_lang_grammar = R"(
%PARSER%

GOAL:
    GLOBAL_DECLARATIONS

GLOBAL_DECLARATIONS:
    GLOBAL_DECLARATION GLOBAL_DECLARATIONS
    | GLOBAL_DECLARATION
    | EPSILON

GLOBAL_DECLARATION:
    FUNCTION_DEFINITION
    | PARSER_RULE
    | LEXER_RULE
    | STATEMENT

LEXER_RULE:
    "lexer" IDENTIFIER
    '{'
        LEXER_RULE_BODY
    '}'

LEXER_RULE_BODY:
    LEXER_RULE_BODY ',' EXPRESSION
    | EXPRESSION
    | EPSILON

PARSER_RULE:
    "parser" IDENTIFIER
    '{'
        PARSER_RULE_BODY
    '}'

PARSER_RULE_BODY:
    PARSER_SYMBOLS ';' PARSER_FUNCTIONS

PARSER_FUNCTIONS:
    PARSER_FUNCTIONS FUNCTION_DEFINITION
    | FUNCTION_DEFINITION
    | EPSILON

PARSER_SYMBOLS:
    PARSER_SYMBOLS ',' EXPRESSION
    | EXPRESSION
    | EPSILON

FUNCTION_DEFINITION:
    "def" IDENTIFIER '(' FUNCTION_DEFINITION_ARGUMENTS ')' FUNCTION_RETURN_TYPE
    '{'
        STATEMENTS
    '}'

FUNCTION_DEFINITION_ARGUMENTS:
    FUNCTION_DEFINITION_ARGUMENT ',' FUNCTION_DEFINITION_ARGUMENTS
    | FUNCTION_DEFINITION_ARGUMENT
    | EPSILON

FUNCTION_DEFINITION_ARGUMENT:
    TYPE IDENTIFIER

FUNCTION_RETURN_TYPE:
    '->' TYPE
    | EPSILON

STATEMENTS:
    STATEMENT STATEMENTS
    | STATEMENT
    | EPSILON

STATEMENT:
    ASSIGNMENT_EXPRESSION ';'
    | VARIABLE_DECLARATION ';'
    | TYPE_VARIABLE_DECLARATION ';'
    | FOR_LOOP
    | WHILE_LOOP
    | IF_STATEMENT
    | RETURN_STATEMENT ';'

RETURN_STATEMENT:
    "return" ASSIGNMENT_EXPRESSION
    | "return"

IF_STATEMENT:
    IF_BLOCK
    ELIF_BLOCK
    ELSE_BLOCK

IF_BLOCK:
    "if" ASSIGNMENT_EXPRESSION
    '{'
        STATEMENTS
    '}'

ELIF_BLOCK:
    "elif" ASSIGNMENT_EXPRESSION
    '{'
        STATEMENTS
    '}'
    ELIF_BLOCK
    | EPSILON

ELSE_BLOCK:
    "else"
    '{'
        STATEMENTS
    '}'
    | EPSILON

LOGICAL_OR_EXPRESSION:
    LOGICAL_OR_EXPRESSION '||' LOGICAL_AND_EXPRESSION
    | LOGICAL_AND_EXPRESSION

LOGICAL_AND_EXPRESSION:
    LOGICAL_AND_EXPRESSION '&&' BITWISE_OR_EXPRESSION
    | BITWISE_OR_EXPRESSION

BITWISE_OR_EXPRESSION:
    BITWISE_OR_EXPRESSION '|' BITWISE_XOR_EXPRESSION
    | BITWISE_XOR_EXPRESSION

BITWISE_XOR_EXPRESSION:
    BITWISE_XOR_EXPRESSION '^' BITWISE_AND_EXPRESSION
    | BITWISE_AND_EXPRESSION

BITWISE_AND_EXPRESSION:
    BITWISE_AND_EXPRESSION '&' EQUALITY
    | EQUALITY

EQUALITY:
    EQUALITY '==' COMPARISON
    | EQUALITY '!=' COMPARISON
    | COMPARISON

COMPARISON:
    COMPARISON '<' EXPRESSION
    | COMPARISON '>' EXPRESSION
    | COMPARISON '<=' EXPRESSION
    | COMPARISON '>=' EXPRESSION
    | EXPRESSION

ASSIGNMENT_EXPRESSION:
    LOGICAL_OR_EXPRESSION '=' ASSIGNMENT_EXPRESSION
    | LOGICAL_OR_EXPRESSION

EXPRESSION:
    EXPRESSION '+' TERM
    | EXPRESSION '-' TERM
    | TERM

TERM:
    TERM '*' VALUE
    | TERM '/' VALUE
    | VALUE

VALUE:
    '-' VALUE
    | '+' VALUE
    | FACTOR

VARIABLE_DECLARATION:
    "var" IDENTIFIER '=' ASSIGNMENT_EXPRESSION

TYPE_VARIABLE_DECLARATION:
    "var" IDENTIFIER ':' TYPE '=' ASSIGNMENT_EXPRESSION

FOR_LOOP:
    "for" IDENTIFIER "in" ASSIGNMENT_EXPRESSION
    '{'
        STATEMENTS
    '}'

WHILE_LOOP:
    "while" ASSIGNMENT_EXPRESSION
    '{'
        STATEMENTS
    '}'

FUNCTION_CALL:
    IDENTIFIER '(' FUNCTION_CALL_ARGUMENTS ')'

FUNCTION_CALL_ARGUMENTS:
    ASSIGNMENT_EXPRESSION ',' FUNCTION_CALL_ARGUMENTS
    | ASSIGNMENT_EXPRESSION
    | EPSILON

METHOD_CALL:
    FACTOR '.' IDENTIFIER '(' FUNCTION_CALL_ARGUMENTS ')'

FIELD_ACCESS:
    FACTOR '.' IDENTIFIER

FACTOR:
    NUMBER
    | FLOAT
    | ARRAY
    | "true"
    | "false"
    | IDENTIFIER
    | STRING
    | FUNCTION_CALL
    | METHOD_CALL
    | FIELD_ACCESS
    | '(' ASSIGNMENT_EXPRESSION ')'

ARRAY:
    '[' ARRAY_ELEMENTS ']'

ARRAY_ELEMENTS:
    ASSIGNMENT_EXPRESSION ',' ARRAY_ELEMENTS
    | ASSIGNMENT_EXPRESSION

TYPE:
    IDENTIFIER
    | IDENTIFIER '<' TYPE '>'

%LEXER%
IDENTIFIER: [a-zA-Z_] [a-zA-Z0-9_]*
FLOAT:      [0-9]+[.][0-9]+
NUMBER:     [0-9]+
STRING:     ! ["] ("\\\"" | ["]^)* ["]
)";
        return ast_lang_grammar;
    }

    auto getNodeTypesMap() -> const std::unordered_map<std::string, SmallId> &
    {
        const static std::unordered_map<std::string, SmallId> node_types_map{ // NOSONAR
            {
                "\"true\"",
                std::to_underlying(NodeTypes::TRUE),
            },
            {
                "\"false\"",
                std::to_underlying(NodeTypes::FALSE),
            },
            {
                "NUMBER",
                std::to_underlying(NodeTypes::NUMBER),
            },
            {
                "FLOAT",
                std::to_underlying(NodeTypes::FLOAT),
            },
            {
                "STRING",
                std::to_underlying(NodeTypes::STRING),
            },
            {
                "IDENTIFIER",
                std::to_underlying(NodeTypes::IDENTIFIER),
            },
            {
                "FUNCTION_CALL",
                std::to_underlying(NodeTypes::FUNCTION_CALL),
            },
            {
                "METHOD_CALL",
                std::to_underlying(NodeTypes::METHOD_CALL),
            },
            {
                "VALUE",
                std::to_underlying(NodeTypes::FACTOR),
            },
            {
                "FACTOR",
                std::to_underlying(NodeTypes::VALUE),
            },
            {
                "TERM",
                std::to_underlying(NodeTypes::TERM),
            },
            {
                "EXPRESSION",
                std::to_underlying(NodeTypes::EXPRESSION),
            },
            {
                "COMPARISON",
                std::to_underlying(NodeTypes::COMPARISON),
            },
            {
                "EQUALITY",
                std::to_underlying(NodeTypes::EQUALITY),
            },
            {
                "BITWISE_AND_EXPRESSION",
                std::to_underlying(NodeTypes::BITWISE_AND_EXPRESSION),
            },
            {
                "BITWISE_XOR_EXPRESSION",
                std::to_underlying(NodeTypes::BITWISE_XOR_EXPRESSION),
            },
            {
                "BITWISE_OR_EXPRESSION",
                std::to_underlying(NodeTypes::BITWISE_OR_EXPRESSION),
            },
            {
                "LOGICAL_AND_EXPRESSION",
                std::to_underlying(NodeTypes::LOGICAL_AND_EXPRESSION),
            },
            {
                "LOGICAL_OR_EXPRESSION",
                std::to_underlying(NodeTypes::LOGICAL_OR_EXPRESSION),
            },
            {
                "ASSIGNMENT_EXPRESSION",
                std::to_underlying(NodeTypes::ASSIGNMENT_EXPRESSION),
            },
            {
                "\'+\'",
                std::to_underlying(NodeTypes::PLUS),
            },
            {
                "\'-\'",
                std::to_underlying(NodeTypes::MINUS),
            },
            {
                "\'*\'",
                std::to_underlying(NodeTypes::MULTIPLY),
            },
            {
                "\'/\'",
                std::to_underlying(NodeTypes::DIVIDE),
            },
            {
                "\'%\'",
                std::to_underlying(NodeTypes::MODULUS),
            },
            {
                "\'<\'",
                std::to_underlying(NodeTypes::LESS),
            },
            {
                "\'<=\'",
                std::to_underlying(NodeTypes::LESS_EQUAL),
            },
            {
                "\'>\'",
                std::to_underlying(NodeTypes::GREATER),
            },
            {
                "\'>=\'",
                std::to_underlying(NodeTypes::GREATER_EQUAL),
            },
            {
                "\'==\'",
                std::to_underlying(NodeTypes::EQUAL),
            },
            {
                "\'!=\'",
                std::to_underlying(NodeTypes::NOT_EQUAL),
            },
            {
                "\'&&\'",
                std::to_underlying(NodeTypes::LOGICAL_AND),
            },
            {
                "\'||\'",
                std::to_underlying(NodeTypes::LOGICAL_OR),
            },
            {
                "\'&\'",
                std::to_underlying(NodeTypes::BITWISE_AND),
            },
            {
                "\'|\'",
                std::to_underlying(NodeTypes::BITWISE_OR),
            },
            {
                "\'^\'",
                std::to_underlying(NodeTypes::BITWISE_XOR),
            },
            {
                "\'~\''",
                std::to_underlying(NodeTypes::BITWISE_NOT),
            },
            {
                "\'=\'",
                std::to_underlying(NodeTypes::ASSIGN),
            },
        };

        return node_types_map;
    }
}// namespace astlang2
