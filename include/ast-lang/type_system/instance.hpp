#ifndef AST_LANG_TYPE_SYSTEM_INSTANCE_HPP
#define AST_LANG_TYPE_SYSTEM_INSTANCE_HPP

#include <ast-lang/type_system/type.hpp>
#include <isl/unique_any.hpp>

namespace astlang
{
    struct Instance
    {
        ankerl::unordered_dense::map<std::string, isl::UniqueAny> fields;
        type_system::Type *type;
    };
}// namespace astlang

#endif /* AST_LANG_TYPE_SYSTEM_INSTANCE_HPP */
