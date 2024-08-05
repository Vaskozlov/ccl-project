#include <ast-lang/interpreter/stack.hpp>

namespace astlang::interpreter
{
    auto Stack::read(const std::string &name) -> EvaluationResult &
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

    auto Stack::createVariable(const std::string &name, EvaluationResult value) -> void
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

    auto Stack::createGlobalVariable(const std::string &name, EvaluationResult value) -> void
    {
        auto [it, has_inserted] = globalScope.try_emplace(name, std::move(value));

        if (!has_inserted) {
            throw std::runtime_error("Variable already exists");
        }
    }

    auto Stack::write(const std::string &name, EvaluationResult value) -> void
    {
        if (localScopes.empty()) {
            return globalWrite(name, std::move(value));
        }

        auto &last_scope = localScopes.back();
        last_scope.at(name) = std::move(value);
    }

    auto Stack::globalWrite(const std::string &name, EvaluationResult value) -> void
    {
        globalScope.at(name) = std::move(value);
    }

    auto Stack::globalRead(const std::string &name) -> EvaluationResult &
    {
        return globalScope.at(name);
    }

    auto Stack::pushHardStackScope() -> void
    {
        localScopes.emplace_back();
    }

    auto Stack::popStackScope() -> void
    {
        localScopes.pop_back();
    }
}// namespace astlang::interpreter