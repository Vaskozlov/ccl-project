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
        isl::ssize_t currentStackTop = -1;

    public:
        auto read(const std::string &name) -> Value;

        auto readGlobal(const std::string &name) -> Value;

        auto write(const std::string &name, Value value, bool check_type = true) -> void;

        auto writeGlobal(const std::string &name, Value value, bool check_type = true) -> void;

        [[nodiscard]] auto getCurrentStackTop() const noexcept -> std::size_t
        {
            return static_cast<std::size_t>(currentStackTop);
        }

        auto createsHardScope() -> auto
        {
            return isl::Raii{
                [this]() {
                ++currentStackTop;

                if (getCurrentStackTop() < localScopes.size()) {
                    localScopes.at(getCurrentStackTop()).isHardScope = true;
                } else {
                    localScopes.emplace_back().isHardScope = true;
                }
            },
                [this]() {
                localScopes.at(getCurrentStackTop()).variables.clear();
                --currentStackTop;
            },
            };
        }

        auto createsSoftScope() -> auto
        {
            return isl::Raii{
                [this]() {
                ++currentStackTop;

                if (getCurrentStackTop() < localScopes.size()) {
                    localScopes.at(getCurrentStackTop()).isHardScope = false;
                } else {
                    localScopes.emplace_back().isHardScope = false;
                }
            },
                [this]() {
                localScopes.at(getCurrentStackTop()).variables.clear();
                --currentStackTop;
            },
            };
        }
    };
} // namespace astlang2::interpreter

#endif /* AST_LANG2_STACK_HPP */
