#ifndef CCL_PROJECT_EVALUATION_RESULT_HPP
#define CCL_PROJECT_EVALUATION_RESULT_HPP

#include <ast-lang/interpreter/type.hpp>
#include <isl/unique_any.hpp>

namespace astlang::interpreter
{
    struct EvaluationResult
    {
        isl::UniqueAny value;
        Type type;
        bool needToReturn{};
        bool storesReference{};
    };
}// namespace astlang::interpreter

namespace astlang
{
    template<typename T>
    auto observe(interpreter::EvaluationResult &result) -> T *
    {
        if (result.storesReference) {
            auto *casted_value = *isl::observe<interpreter::EvaluationResult *>(result.value);

            if (casted_value->storesReference) {
                return observe<T>(*casted_value);
            }

            return isl::observe<T>(casted_value->value);
        }

        return isl::observe<T>(result.value);
    }
}// namespace astlang

#endif /* CCL_PROJECT_EVALUATION_RESULT_HPP */
