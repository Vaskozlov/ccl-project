#ifndef CCL_PROJECT_STACK_HPP
#define CCL_PROJECT_STACK_HPP

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
        auto read(const std::string &name) -> EvaluationResult &
        {
            if (localScopes.empty()) {
                return globalRead(name);
            }

            auto &last_scope = localScopes.back();

            if (auto it = last_scope.find(name); it != last_scope.end()) {
                return it->second;
            }

            return globalRead(name);
        }

        auto createVariable(const std::string &name, EvaluationResult value) -> void
        {
            if (localScopes.empty()) {
                return createGlobalVariable(name, std::move(value));
            }

            auto &last_scope = localScopes.back();
            auto [it, has_inserted] = last_scope.try_emplace(name, std::move(value));

            if (!has_inserted) {
                throw std::runtime_error("Variable already exists");
            }
        }

        auto createGlobalVariable(const std::string &name, EvaluationResult value) -> void
        {
            auto [it, has_inserted] = globalScope.try_emplace(name, std::move(value));

            if (!has_inserted) {
                throw std::runtime_error("Variable already exists");
            }
        }

        auto write(const std::string &name, EvaluationResult value) -> void
        {
            if (localScopes.empty()) {
                return globalWrite(name, std::move(value));
            }

            auto &last_scope = localScopes.back();
            last_scope.at(name) = std::move(value);
        }

        auto globalWrite(const std::string &name, EvaluationResult value) -> void
        {
            globalScope.at(name) = std::move(value);
        }

        auto globalRead(const std::string &name) -> EvaluationResult &
        {
            return globalScope.at(name);
        }

        auto pushHardStackScope() -> void
        {
            localScopes.emplace_back();
        }

        auto popStackScope() -> void
        {
            localScopes.pop_back();
        }
    };
}// namespace astlang::interpreter

#endif /* CCL_PROJECT_STACK_HPP */
