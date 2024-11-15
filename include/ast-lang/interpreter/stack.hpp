#ifndef CCL_PROJECT_ASTLANG_STACK_HPP
#define CCL_PROJECT_ASTLANG_STACK_HPP

#include <ankerl/unordered_dense.h>
#include <ast-lang/interpreter/evaluation_result.hpp>
#include <ast-lang/type_system/instance.hpp>
#include <isl/raii.hpp>

namespace astlang::interpreter
{
    class Stack
    {
    private:
        ankerl::unordered_dense::map<std::string, EvaluationResult> globalScope;
        std::vector<ankerl::unordered_dense::map<std::string, EvaluationResult>> localScopes;

    public:
        auto read(const std::string &name) -> EvaluationResult &;

        auto createVariable(const std::string &name, EvaluationResult value) -> void;

        auto createGlobalVariable(const std::string &name, EvaluationResult value) -> void;

        auto write(const std::string &name, EvaluationResult value) -> void;

        auto globalWrite(const std::string &name, EvaluationResult value) -> void;

        auto globalRead(const std::string &name) -> EvaluationResult &;

        auto createsHardScope() -> auto
        {
            return isl::Raii{
                [this]() {
                    localScopes.emplace_back();
                },
                [this]() {
                    localScopes.pop_back();
                },
            };
        }
    };
}// namespace astlang::interpreter

#endif /* CCL_PROJECT_ASTLANG_STACK_HPP */
