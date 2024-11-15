#ifndef CCL_PROJECT_ASTLANG_FUNCTIONS_HPP
#define CCL_PROJECT_ASTLANG_FUNCTIONS_HPP

#include <ast-lang/interpreter/function_interface.hpp>

namespace astlang::interpreter
{
    struct FunctionIdentification
    {
        std::string name;
        type_system::Type *returnType;
        std::vector<type_system::Type *> parameters;

        auto operator==(const FunctionIdentification &other) const -> bool
        {
            return name == other.name && parameters == other.parameters;
        }

        auto operator<=>(const FunctionIdentification &other) const -> std::weak_ordering
        {
            if (const auto cmp = name <=> other.name; cmp != 0) {
                return cmp;
            }

            return parameters <=> other.parameters;
        }
    };

    class Functions
    {
    private:
        std::map<FunctionIdentification, std::unique_ptr<FunctionInterface>> functions;

    public:
        auto addFunction(
            FunctionIdentification identification,
            std::unique_ptr<FunctionInterface>
                function) -> void
        {
            auto has_inserted = false;

            std::tie(std::ignore, has_inserted) =
                functions.try_emplace(std::move(identification), std::move(function));

            if (!has_inserted) {
                throw std::runtime_error("Function already exists");
            }
        }

        auto getFunction(const FunctionIdentification &identification) -> FunctionInterface *
        {
            const auto it = functions.find(identification);

            if (it == functions.end()) {
                throw std::runtime_error("Function not found");
            }

            return it->second.get();
        }
    };
}// namespace astlang::interpreter

#endif /* CCL_PROJECT_ASTLANG_FUNCTIONS_HPP */
