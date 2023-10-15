#ifndef CCL_PROJECT_C_RULE_HPP
#define CCL_PROJECT_C_RULE_HPP

#include <isl/string_view.hpp>

namespace ccl::debug
{
    constexpr inline isl::string_view C_RULE = R"(
VAR_NAME = "CLexer"

COMMENT: ! "//"[\n]*^
COMMENT: ! "/*" "*/"^* "*/"

[SEPARATOR]
CURLY_OPENING:      ! [{]
CURLY_CLOSING:      ! [}]
LEFT_PARENTHESIS:   ! [(]
RIGHT_PARENTHESIS:  ! [)]
ANGLE_OPENING:      ! [\[]
ANGLE_CLOSING:      ! [\]]
COMMA:              ! [,]
COLUMN:             ! [;]
PREPROCESSOR:       ! [#]
QUESTION_MARK:      ! [?]

[OPERATORS]
ADD:                ! [+]
SUB:                ! [\-]
MUL:                ! [*]
DIV:                ! [/]
MOD:                ! [%]
ASSIGN:             ! [=]

ADD_ASSIGN:         ! "+="
SUB_ASSIGN:         ! "-="
MUL_ASSIGN:         ! "*="
DIB_ASSIGN:         ! "/="
MOD_ASSIGN:         ! "%="

OR:                 ! [|]
AND:                ! [&]
XOR:                ! [^]
BITWISE_NOT:        ! [~]

OR_ASSIGN:          ! "|="
AND_ASSIGN:         ! "&="
XOR_ASSIGN:         ! "^="

LESS:               ! "<"
GREATER:            ! ">"

EQUAL:              ! "=="
NOT_EQUAL:          ! "!="
LESS_EQUAL:         ! "<="
GREATER_EQUAL:      ! ">="

LOGICAL_OR:         ! "||"
LOGICAL_AND:        ! "&&"
LOGICAL_NOT:        ! [!]

[KEYWORD]
FOR:                "for"
WHILE:              "while"
SWITCH:             "switch"
CASE:               "case"
RETURN:             "return"
STRUCT:             "struct"

[DECLS]
NUMBER:             [0-9]+ ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p
FLOAT:              [0-9]+"."[0-9]* ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p
IDENTIFIER:         [a-zA-Z_]+[a-zA-Z0-9_]*
CHAR:               ! "\'" ([']^ | "\\\'" ) "\'"
STRING:             ! "\"" (["]^ | "\\\"")* "\""
)";
}

#endif /* CCL_PROJECT_C_RULE_HPP */
