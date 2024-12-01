#ifndef AST_LANG_2_TS_VALUE_HPP
#define AST_LANG_2_TS_VALUE_HPP

#include <ast-lang-2/ast-lang.hpp>

namespace astlang2
{
    namespace ts
    {
        class Type;
    }

    enum class ValueType
    {
        R_VALUE,
        L_VALUE,
    };

    namespace detail
    {
        using ObjectSharedPtrFrame = isl::SharedPtrFrame<
            isl::ObjectsMaxSize<
                std::string, ankerl::unordered_dense::map<std::string, std::string>>,
            isl::ObjectsMaxAlignment<
                std::string, ankerl::unordered_dense::map<std::string, std::string>>>;

        extern isl::PoolAllocator<sizeof(ObjectSharedPtrFrame), alignof(ObjectSharedPtrFrame)>
            ObjectAllocator;
    }// namespace detail

    template<typename T = void>
    using AstlangObject =
        isl::SharedPtr<T, detail::ObjectSharedPtrFrame, std::addressof(detail::ObjectAllocator)>;

    struct Value
    {
        AstlangObject<> object;
        ts::Type *type{};
        ValueType valueType = ValueType::R_VALUE;
    };

    using Instance = ankerl::unordered_dense::map<std::string, Value>;

    template<typename T>
    auto toAstlangObject(T &&value) -> AstlangObject<>
    {
        return AstlangObject<std::remove_cvref_t<T>>{std::forward<T>(value)};
    }
}// namespace astlang2

#endif /* AST_LANG_2_TS_VALUE_HPP */
