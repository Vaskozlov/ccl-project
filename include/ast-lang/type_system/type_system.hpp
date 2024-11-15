#ifndef ASTLANG_TYPE_SYSTEM_TYPE_SYSTEM_HPP
#define ASTLANG_TYPE_SYSTEM_TYPE_SYSTEM_HPP

#include <ast-lang/ast/node.hpp>
#include <ast-lang/type_system/type.hpp>
#include <ccl/parser/ast/node.hpp>
#include <isl/id_generator.hpp>

namespace astlang::type_system
{
    class TypeSystem
    {
    private:
        ankerl::unordered_dense::map<std::string, std::unique_ptr<Type>> typeMap;
        isl::IdGenerator<ccl::Id> typeIdGenerator{0U};

    public:
        TypeSystem()
        {
            // typeMap.emplace("_error", std::make_unique<Type>("_error"));
            // typeMap.emplace("void", std::make_unique<Type>("void"));
            //
            // typeMap.emplace("bool", std::make_unique<Type>("bool"));
            //
            // typeMap.emplace("char", std::make_unique<Type>("char"));
            // typeMap.emplace("int", std::make_unique<Type>("int"));
            // typeMap.emplace("uint", std::make_unique<Type>("uint"));
            //
            // typeMap.emplace("float", std::make_unique<Type>("float"));
            // typeMap.emplace("double", std::make_unique<Type>("double"));
            //
            // typeMap.emplace("string", std::make_unique<Type>("string"));
        }

        auto createType(const std::string &name) -> Type *
        {
            if (typeMap.contains(name)) {
                return typeMap[name].get();
            }

            // auto [it, _] = typeMap.emplace(name, std::make_unique<Type>(name));
            // it->second->id = typeIdGenerator.next();

            // return it->second.get();
            return nullptr;
        }

        auto getType(const ccl::parser::ast::Node *node) const -> Type *;

        auto getType(const std::string &name) const -> Type *
        {
            auto it = typeMap.find(name);

            if (it == typeMap.end()) {
                return nullptr;
            }

            return it->second.get();
        }
    };
}// namespace astlang::type_system

#endif /* ASTLANG_TYPE_SYSTEM_TYPE_SYSTEM_HPP */
