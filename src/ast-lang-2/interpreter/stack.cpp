#include <ast-lang-2/interpreter/stack.hpp>

namespace astlang2::interpreter
{
    static auto
        writeVariable(Scope &scope, const std::string &name, Value value, const bool check_type)
            -> void
    {
        auto it = scope.variables.find(name);
        value.valueType = ValueType::L_VALUE;

        if (it != scope.variables.end()) {
            if (check_type && it->second.type != value.type) {
                throw std::runtime_error("Type mismatch");
            }

            it->second = std::move(value);
            return;
        }

        scope.variables.emplace(name, std::move(value));
    }

    auto Stack::read(const std::string &name) -> Value
    {
        for (auto &[variables, is_hard_scope] : localScopes | std::views::reverse) {
            const auto it = variables.find(name);

            if (it != variables.end()) {
                return it->second;
            }

            if (is_hard_scope) {
                break;
            }
        }

        return readGlobal(name);
    }

    auto Stack::readGlobal(const std::string &name) -> Value
    {
        const auto it = globalScope.variables.find(name);

        if (it == globalScope.variables.end()) {
            throw std::runtime_error("Variable not found");
        }

        return it->second;
    }

    auto Stack::write(const std::string &name, Value value, const bool check_type) -> void
    {
        if (localScopes.empty()) {
            writeGlobal(name, std::move(value));
            return;
        }

        auto &last_scope = localScopes.back();
        writeVariable(last_scope, name, std::move(value), check_type);
    }

    auto Stack::writeGlobal(const std::string &name, Value value, const bool check_type) -> void
    {
        writeVariable(globalScope, name, std::move(value), check_type);
    }
}// namespace astlang2::interpreter
