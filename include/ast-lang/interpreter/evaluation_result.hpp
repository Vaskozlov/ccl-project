#ifndef CCL_PROJECT_ASTLANG_EVALUATION_RESULT_HPP
#define CCL_PROJECT_ASTLANG_EVALUATION_RESULT_HPP

#include <ast-lang/type_system/type.hpp>
#include <ast-lang/type_system/instance.hpp>
#include <isl/unique_any.hpp>

namespace astlang::interpreter
{
    struct EvaluationResult
    {
        Instance instance;
        bool needToReturn{};
        bool storesReference{};
    };
}// namespace astlang::interpreter

namespace astlang
{
    // template<typename T>
    // auto observe(interpreter::EvaluationResult &result) -> T *
    // {
    //     if (result.storesReference) {
    //         auto *casted_value = isl::get<interpreter::EvaluationResult *>(result.value);
    //
    //         if (casted_value->storesReference) {
    //             return observe<T>(*casted_value);
    //         }
    //
    //         return isl::observe<T>(casted_value->value);
    //     }
    //
    //     return isl::observe<T>(result.value);
    // }
}// namespace astlang

#endif /* CCL_PROJECT_ASTLANG_EVALUATION_RESULT_HPP */
