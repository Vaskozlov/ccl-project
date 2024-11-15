#ifndef AST_LANG_TYPE_SYSTEM_FIELD_HPP
#define AST_LANG_TYPE_SYSTEM_FIELD_HPP

#include <ast-lang/ast-lang.hpp>

namespace astlang::type_system
{
    struct Type;

    enum class FieldType
    {
        PRIVATE,
        PUBLIC,
        PROTECTED,
    };

    struct Field
    {
        std::string name{};
        Type *type{};
        FieldType fieldType{};
        bool isStatic = false;
    };
}// namespace astlang::type_system

#endif /* AST_LANG_TYPE_SYSTEM_FIELD_HPP */
