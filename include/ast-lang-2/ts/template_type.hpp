#ifndef AST_LANG_2_TEMPLATE_TYPE_HPP
#define AST_LANG_2_TEMPLATE_TYPE_HPP

#include <ast-lang-2/ts/type.hpp>

namespace astlang2::function
{
    class Function;
}

namespace astlang2::ts
{
    class TemplateType;

    using AnyType = std::variant<Type *, TemplateType *>;

    class TemplateType
    {
    private:
        template<typename... Ts>
        class Visitor : public Ts...
        {
        public:
            using Ts::operator()...;
        };

        std::string name;

        ankerl::unordered_dense::map<std::string, std::function<Field()>> fieldsCreator;

        ankerl::unordered_dense::
            map<std::string, std::vector<std::function<function::Function *()>>>
                methodsCreator;

        std::vector<AnyType> templateParameters;
        std::vector<std::size_t> prototypeTemplateParameterIndices;

        AnyType prototype = static_cast<Type *>(nullptr);
        ccl::Id id{};

    public:
        TemplateType(std::string type_name, const ccl::Id type_id, Type *parent_type = nullptr)
          : name{std::move(type_name)}
          , prototype{parent_type}
          , id{type_id}
        {}

        [[nodiscard]] auto operator==(const TemplateType &other) const -> bool
        {
            return id == other.id;
        }

        [[nodiscard]] auto operator<=>(const TemplateType &other) const -> std::weak_ordering
        {
            return id <=> other.id;
        }

        auto addField(const std::string &field_name, std::function<Field()> field_creator) -> void
        {
            auto inserted = false;
            std::tie(std::ignore, inserted) = fieldsCreator.try_emplace(field_name, field_creator);

            if (!inserted) {
                throw std::runtime_error(fmt::format("Field '{}' already exists", field_name));
            }
        }

        [[nodiscard]] auto getTemplateTypes() const -> const std::vector<AnyType> &
        {
            return templateParameters;
        }

        [[nodiscard]] auto hasPrototype() const -> bool
        {
            return std::visit(
                [](const auto *type) {
                    return type != nullptr;
                },
                prototype);
        }

        [[nodiscard]] auto getPrototype() const -> AnyType
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

        [[nodiscard]] auto hasField(const std::string &field_name) const -> bool
        {
            return fieldsCreator.contains(field_name);
        }
    };
}// namespace astlang2::ts

#endif /* AST_LANG_2_TEMPLATE_TYPE_HPP */
