#ifndef ASTLANG_TYPE_SYSTEM_TYPE_HPP
#define ASTLANG_TYPE_SYSTEM_TYPE_HPP

#include <ankerl/unordered_dense.h>
#include <ast-lang/ast-lang.hpp>
#include <ast-lang/type_system/field.hpp>

namespace astlang::interpreter
{
    class FunctionInterface;
}

namespace astlang::type_system
{
    class Type
    {
    private:
        std::string name;
        std::vector<Type> templateParameters;
        ankerl::unordered_dense::map<std::string, Field> fields;
        ankerl::unordered_dense::map<std::string, std::vector<interpreter::FunctionInterface *>>
            methods;
        Type *prototype{};
        ccl::Id id{};

    public:
        [[nodiscard]] auto operator==(const Type &other) const noexcept -> bool
        {
            return id == other.id;
        }

        [[nodiscard]] auto operator<=>(const Type &other) const -> std::weak_ordering
        {
            return id <=> other.id;
        }

        auto addField(Field field) -> void
        {
            bool inserted = false;
            std::tie(std::ignore, inserted) = fields.try_emplace(field.name, field);

            if (!inserted) {
                throw std::runtime_error(fmt::format("Field '{}' already exists", field.name));
            }
        }

        [[nodiscard]] auto getField(const std::string &name) -> Field
        {
            const auto it = fields.find(name);

            if (it == fields.end()) {
                throw std::runtime_error(fmt::format("Field '{}' does not exist", name));
            }

            return it->second;
        }

        [[nodiscard]] auto getMethod(const std::string &name)
            -> std::vector<interpreter::FunctionInterface *>
        {
            const auto it = methods.find(name);

            if (it == methods.end()) {
                throw std::runtime_error(fmt::format("Method '{}' does not exist", name));
            }

            return it->second;
        }

        auto addMethod(const std::string &name, interpreter::FunctionInterface *method) -> void
        {
            bool inserted = false;
            // std::tie(std::ignore, inserted) = methods.try_emplace(name, method);

            if (!inserted) {
                throw std::runtime_error(
                    fmt::format(
                        "Method '{}:{}' already exists", name, static_cast<void *>(method)));
            }
        }

        [[nodiscard]] auto getTemplateParameters() const noexcept -> const std::vector<Type> &
        {
            return templateParameters;
        }

        [[nodiscard]] auto getPrototype() const noexcept -> Type *
        {
            return prototype;
        }

        [[nodiscard]] auto hasPrototype() const noexcept -> bool
        {
            return prototype != nullptr;
        }

        [[nodiscard]] auto getId() const noexcept -> ccl::Id
        {
            return id;
        }

        [[nodiscard]] auto getName() const noexcept -> const std::string &
        {
            return name;
        }

        [[nodiscard]] auto hasField(const std::string &name) const -> bool
        {
            return fields.contains(name);
        }

        [[nodiscard]] auto hasMethod(const std::string &name) const -> bool
        {
            return methods.contains(name);
        }
    };
}// namespace astlang::type_system

template<>
struct fmt::formatter<astlang::type_system::Type> : formatter<std::string>
{
    auto format(const astlang::type_system::Type &type, format_context &ctx) const
        -> format_context::iterator
    {
        return formatter<std::string>::format(type.getName(), ctx);
    }
};

#endif /* ASTLANG_TYPE_SYSTEM_TYPE_HPP */
