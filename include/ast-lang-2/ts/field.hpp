#ifndef AST_LANG_2_FIELD_HPP
#define AST_LANG_2_FIELD_HPP

namespace astlang2::ts
{
    class Type;
    class TemplateType;

    struct Field
    {
        std::string name;
        Type *type{};
    };
}// namespace astlang2::ts

#endif /* AST_LANG_2_FIELD_HPP */
