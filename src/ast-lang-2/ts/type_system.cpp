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
        new_type->addField(ts::Field{"_value", builtinType});

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

    auto TypeSystem::getTypeFromNode(const ast::core::AstlangNode *node) -> ts::Type *
    {
        const auto *main_type_node =
            static_cast<const ccl::parser::ast::Terminal *>(node->front().get());

        const ccl::lexer::Token &main_type_token = main_type_node->getToken();
        const auto main_type_repr = main_type_token.getRepr();

        if (node->size() == 1) {
            return getType(static_cast<std::string>(main_type_repr));
        }

        throw std::runtime_error{"Templates are not supported yet"};
    }
}// namespace astlang2
