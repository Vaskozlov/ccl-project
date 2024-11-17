#ifndef ASTLANG_2_TYPE_HPP
#define ASTLANG_2_TYPE_HPP

#include <ankerl/unordered_dense.h>
#include <ast-lang-2/ast-lang.hpp>
#include <ast-lang-2/function/function.hpp>
#include <ast-lang-2/function/functions_holder.hpp>
#include <ast-lang-2/ts/field.hpp>

namespace astlang2::function
{
    class Function;
}

namespace astlang2::ts
{
    class Type
    {
    private:
        std::string name;
        ankerl::unordered_dense::map<std::string, Field> fields;
        function::FunctionsHolder methods;
        std::vector<Type *> templateParameters;
        ccl::Id id{};
        Type *prototype{};
        bool builtin{false};

    public:
        Type(
            std::string type_name, ccl::Id type_id, Type *parent_type = nullptr,
            bool is_builtin = false);

        [[nodiscard]] auto operator==(const Type &other) const -> bool
        {
            return id == other.id;
        }

        [[nodiscard]] auto operator<=>(const Type &other) const -> std::weak_ordering
        {
            return id <=> other.id;
        }

        auto addField(Field field) -> Field *;

        auto addMethod(
            function::FunctionIdentification identification,
            std::shared_ptr<function::Function>
                function) -> void
        {
            methods.addFunction(std::move(identification), std::move(function));
        }

        auto callMethod(
            interpreter::Interpreter &interpreter, const std::string &method_name,
            const isl::SmallVector<Value, 4> &arguments) -> Value;

        [[nodiscard]] auto getField(Field field) const -> const Field *;

        [[nodiscard]] auto getTemplateTypes() const -> const std::vector<Type *> &
        {
            return templateParameters;
        }

        [[nodiscard]] auto hasPrototype() const -> bool
        {
            return prototype != nullptr;
        }

        [[nodiscard]] auto getPrototype() const -> const Type *
        {
            return prototype;
        }

        [[nodiscard]] auto getName() const -> const std::string &
        {
            return name;
        }

        [[nodiscard]] auto getId() const -> ccl::Id
        {
            return id;
        }

        [[nodiscard]] auto isBuiltin() const noexcept -> bool
        {
            return builtin;
        }

        [[nodiscard]] auto hasField(const std::string &field_name) const -> bool
        {
            return fields.contains(field_name);
        }
    };

}// namespace astlang2::ts

#endif /* ASTLANG_2_TYPE_HPP */
