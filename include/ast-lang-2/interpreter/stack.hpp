#ifndef AST_LANG2_STACK_HPP
#define AST_LANG2_STACK_HPP

#include <ast-lang-2/ts/type.hpp>
#include <isl/raii.hpp>

namespace astlang2::interpreter
{
    struct Scope
    {
        ankerl::unordered_dense::map<std::string, Value> variables{};
        bool isHardScope{};
    };

    class Stack
    {
    private:
        Scope globalScope{.isHardScope = true};
        std::vector<Scope> localScopes;

    public:
        auto read(const std::string &name) -> Value;

        auto readGlobal(const std::string &name) -> Value;

        auto write(const std::string &name, Value value, bool check_type = true) -> void;

        auto writeGlobal(const std::string &name, Value value, bool check_type = true) -> void;

        auto createsHardScope() -> auto
        {
            return isl::Raii{
                [this]() {
                    localScopes.push_back(Scope{.isHardScope = true});
                },
                [this]() {
                    localScopes.pop_back();
                },
            };
        }

        auto createsSoftScope() -> auto
        {
            return isl::Raii{
                [this]() {
                    localScopes.push_back(Scope{.isHardScope = false});
                },
                [this]() {
                    localScopes.pop_back();
                },
            };
        }
    };
}// namespace astlang2::interpreter

#endif /* AST_LANG2_STACK_HPP */
