#ifndef AST_LANG_2_TS_VALUE_HPP
#define AST_LANG_2_TS_VALUE_HPP

#include <ast-lang-2/ast-lang.hpp>

namespace astlang2
{
    namespace ts
    {
        class Type;
    }

    using Instance = ankerl::unordered_dense::map<std::string, std::shared_ptr<void>>;

    enum class ValueType
    {
        R_VALUE,
        L_VALUE,
    };

    struct Value
    {
        std::shared_ptr<void> object;
        ts::Type *type{};
        ValueType valueType = ValueType::R_VALUE;
    };
}// namespace astlang2

#endif /* AST_LANG_2_TS_VALUE_HPP */
