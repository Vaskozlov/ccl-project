#include <ast-lang-2/ast/core/node.hpp>
#include <ast-lang-2/ts/type_system.hpp>

namespace astlang2
{
    auto
        TypeSystem::createTypeAndGetPointer(const std::string &name, std::unique_ptr<ts::Type> type)
            -> ts::Type *
    {
        auto [it, inserted] = types.emplace(name, std::move(type));
        return it->second.get();
    }

    auto TypeSystem::createCoreType(const std::string &name) -> ts::Type *
    {
        auto new_type = std::make_unique<ts::Type>(name, typeIdGenerator.next(), nullptr, true);

        new_type->addField(
            ts::Field{
                .name = "_value",
                .type = builtinType,
            });

        return types.emplace(name, std::move(new_type)).first->second.get();
    }

    TypeSystem::TypeSystem()
    {
        notValidType = createTypeAndGetPointer(
            "__error", std::make_unique<ts::Type>("__error", typeIdGenerator.next()));

        builtinType = createTypeAndGetPointer(
            "__builtin", std::make_unique<ts::Type>("__builtin", typeIdGenerator.next()));

        voidType = createTypeAndGetPointer(
            "void", std::make_unique<ts::Type>("void", typeIdGenerator.next()));

        anyType = createTypeAndGetPointer(
            "any", std::make_unique<ts::Type>("any", typeIdGenerator.next()));

        boolType = createCoreType("bool");
        charType = createCoreType("char");
        intType = createCoreType("int");
        uintType = createCoreType("uint");
        floatType = createCoreType("float");
        doubleType = createCoreType("double");
        stringType = createCoreType("string");
        vec2Type = createCoreType("vec2");
        vec3Type = createCoreType("vec3");

        vec2Type->addField(ts::Field{.name = "x", .type = doubleType});
        vec2Type->addField(ts::Field{.name = "y", .type = doubleType});

        vec3Type->addField(ts::Field{.name = "x", .type = doubleType});
        vec3Type->addField(ts::Field{.name = "y", .type = doubleType});
        vec3Type->addField(ts::Field{.name = "z", .type = doubleType});

        auto list_type = std::make_unique<ts::TemplateType>("list", typeIdGenerator.next());
        templateTypes.emplace(list_type->getName(), std::move(list_type));
    }

    auto TypeSystem::getType(const std::string &name) const -> ts::Type *
    {
        const auto it = types.find(name);

        if (it == types.end()) {
            throw std::runtime_error{fmt::format("Unknown type: {}", name)};
        }

        return it->second.get();
    }

    auto TypeSystem::createType(const std::string &name) -> ts::Type *
    {
        auto [it, inserted] =
            types.emplace(name, std::make_unique<ts::Type>(name, typeIdGenerator.next()));

        if (!inserted) {
            throw std::runtime_error{fmt::format("Type {} already exists", name)};
        }

        return it->second.get();
    }

    auto TypeSystem::getTypeFromNode(const ccl::parser::ast::NonTerminal *node) -> ts::Type *
    {
        const auto *main_type_node =
            static_cast<const ccl::parser::ast::Terminal *>(node->front().get());

        const ccl::lexer::Token &main_type_token = main_type_node->getToken();
        const auto main_type_repr = static_cast<std::string>(main_type_token.getRepr());

        if (node->size() == 1) {
            return getType(main_type_repr);
        }

        const auto *template_argument_node =
            static_cast<const ccl::parser::ast::NonTerminal *>(node->at(2).get());

        const auto *template_type = getTypeFromNode(template_argument_node);
        const auto full_type_name = fmt::format("{}<{}>", main_type_repr, template_type->getName());

        if (types.contains(full_type_name)) {
            return types.at(full_type_name).get();
        }

        const auto *type_to_instance = templateTypes.at(main_type_repr).get();
        auto new_type = std::make_unique<ts::Type>(full_type_name, typeIdGenerator.next());

        for (const auto &[field_name, field_creator] : type_to_instance->getFieldsCreator()) {
            new_type->addField(field_creator(field_name, new_type.get()));
        }

        for (const auto &[method_name, method_creators] : type_to_instance->getMethodsCreator()) {
            for (const auto &method_creator : method_creators) {
                const auto [function_identification, method] =
                    method_creator(method_name, new_type.get());
                new_type->addMethod(function_identification, method);
            }
        }

        auto *new_type_ptr = new_type.get();
        types.emplace(full_type_name, std::move(new_type));

        return new_type_ptr;
    }
}// namespace astlang2
