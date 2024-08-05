#ifndef CCL_PROJECT_ASTLANG_STACK_HPP
#define CCL_PROJECT_ASTLANG_STACK_HPP

#include <ankerl/unordered_dense.h>
#include <ast-lang/interpreter/evaluation_result.hpp>

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

        auto pushHardStackScope() -> void;

        auto popStackScope() -> void;
    };
}// namespace astlang::interpreter

#endif /* CCL_PROJECT_ASTLANG_STACK_HPP */
