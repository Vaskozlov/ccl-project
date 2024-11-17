#ifndef AST_LANG_2_SINGLE_OBJECT_UTILITY_HPP
#define AST_LANG_2_SINGLE_OBJECT_UTILITY_HPP

#include <ast-lang-2/ast-lang.hpp>

namespace astlang2::interpreter
{
    template<typename T>
    struct DoCast
    {
        template<typename U>
        auto operator()(U &&value) -> auto
        {
            return static_cast<T>(std::forward<decltype(value)>(value));
        }
    };

    struct DoToStr
    {
        template<typename U>
        auto operator()(U &&value) -> auto
        {
            return fmt::to_string(value);
        }
    };

    template<typename T>
    struct DoGetLength
    {
        template<typename U>
        auto operator()(U &&value) -> T
        {
            return std::size(value);
        }
    };

    template<typename T, typename Op>
    class OnObjectOperation;

    template<typename Op>
    class OnObjectOperation<void, Op>
    {
    public:
        template<typename U>
        auto operator()(ts::Type *return_type, std::shared_ptr<U> value) const -> auto
        {
            Op{}(*value);

            return Value{
                .object = nullptr,
                .type = return_type,
                .valueType = ValueType::R_VALUE,
            };
        }
    };

    template<typename T, typename Op>
    class OnObjectOperation
    {
    public:
        template<typename U>
        auto operator()(ts::Type *return_type, std::shared_ptr<U> value) const -> auto
        {
            return Value{
                .object = std::make_shared<T>(Op{}(*value)),
                .type = return_type,
                .valueType = ValueType::R_VALUE,
            };
        }
    };

    template<typename T>
    struct CastOperation : OnObjectOperation<T, DoCast<T>>
    {
        using OnObjectOperation<T, DoCast<T>>::OnObjectOperation;
    };

    template<typename T>
    struct GetLengthOperation : OnObjectOperation<T, DoGetLength<T>>
    {
        using OnObjectOperation<T, DoGetLength<T>>::OnObjectOperation;
    };

    template<typename T>
    struct NegateOperation : OnObjectOperation<T, std::negate<T>>
    {
        using OnObjectOperation<T, std::negate<T>>::OnObjectOperation;
    };

    template<typename T>
    struct PositiveOperation : OnObjectOperation<T, DoCast<T>>
    {
        using OnObjectOperation<T, DoCast<T>>::OnObjectOperation;
    };

    struct ToStringOperation : OnObjectOperation<void, DoToStr>
    {
        using OnObjectOperation::OnObjectOperation;
    };
}// namespace astlang2::interpreter

#endif /* AST_LANG_2_SINGLE_OBJECT_UTILITY_HPP */
