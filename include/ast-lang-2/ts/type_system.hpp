#ifndef AST_LANG_2_TYPE_SYSTEM_HPP
#define AST_LANG_2_TYPE_SYSTEM_HPP

#include <ast-lang-2/ts/template_type.hpp>
#include <isl/id_generator.hpp>

namespace ccl::parser::ast
{
    class NonTerminal;
}

namespace astlang2
{
    namespace ast::core
    {
        class AstlangNode;
    }

    class TypeSystem
    {
    private:
        ankerl::unordered_dense::map<std::string, std::unique_ptr<ts::Type>> types;
        ankerl::unordered_dense::map<std::string, std::unique_ptr<ts::TemplateType>> templateTypes;

        isl::IdGenerator<ccl::Id> typeIdGenerator{0U};

        ts::Type *notValidType{nullptr};
        ts::Type *voidType{nullptr};
        ts::Type *builtinType{nullptr};
        ts::Type *anyType{nullptr};
        ts::Type *intType{nullptr};
        ts::Type *uintType{nullptr};
        ts::Type *boolType{nullptr};
        ts::Type *charType{nullptr};
        ts::Type *floatType{nullptr};
        ts::Type *doubleType{nullptr};
        ts::Type *stringType{nullptr};
        ts::TemplateType *listTemplate{nullptr};

        auto createTypeAndGetPointer(const std::string &name, std::unique_ptr<ts::Type> type)
            -> ts::Type *;

        auto createCoreType(const std::string &name) -> ts::Type *;

    public:
        TypeSystem();

        auto getTypeFromNode(const ccl::parser::ast::NonTerminal *node) -> ts::Type *;

        [[nodiscard]] auto getBuiltinType() const noexcept -> ts::Type *
        {
            return builtinType;
        }

        [[nodiscard]] auto getAny() const noexcept -> ts::Type *
        {
            return anyType;
        }

        [[nodiscard]] auto getInt() const noexcept -> ts::Type *
        {
            return intType;
        }

        [[nodiscard]] auto getUint() const noexcept -> ts::Type *
        {
            return uintType;
        }

        [[nodiscard]] auto getBool() const noexcept -> ts::Type *
        {
            return boolType;
        }

        [[nodiscard]] auto getChar() const noexcept -> ts::Type *
        {
            return charType;
        }

        [[nodiscard]] auto getFloat() const noexcept -> ts::Type *
        {
            return floatType;
        }

        [[nodiscard]] auto getDouble() const noexcept -> ts::Type *
        {
            return doubleType;
        }

        [[nodiscard]] auto getString() const noexcept -> ts::Type *
        {
            return stringType;
        }

        [[nodiscard]] auto getVoid() const noexcept -> ts::Type *
        {
            return voidType;
        }

        [[nodiscard]] auto getType(const std::string &name) const -> ts::Type *;

        auto createType(const std::string &name) -> ts::Type *;
    };
}// namespace astlang2

#endif /* AST_LANG_2_TYPE_SYSTEM_HPP */
