#ifndef CCL_PROJECT_ASTLANG_INTERPRETER_TYPE_HPP
#define CCL_PROJECT_ASTLANG_INTERPRETER_TYPE_HPP

#include <ast-lang/ast-lang.hpp>
#include <ast-lang/ast/node.hpp>
#include <ccl/parser/ast/node.hpp>
#include <isl/flatmap.hpp>
#include <isl/string_view.hpp>
#include <list>

namespace astlang::interpreter
{
    class Type
    {
    public:
        enum CoreType : SmallId
        {
            ERROR,
            ANY,
            VOID,
            INT,
            UINT,
            FLOAT,
            STRING,
            UNION,
            LIST,
        };

    private:
        static constexpr isl::StaticFlatmap<CoreType, isl::string_view, 9> CoreTypeToString{
            {
                {ERROR, "error"},
                {INT, "int"},
                {UINT, "uint"},
                {FLOAT, "float"},
                {STRING, "string"},
                {UNION, "union"},
                {LIST, "list"},
                {VOID, "void"},
                {ANY, "any"},
            },
        };

        static constexpr isl::StaticFlatmap<isl::string_view, CoreType, 8> StringToCoreType{
            {
                {"int", INT},
                {"uint", UINT},
                {"float", FLOAT},
                {"string", STRING},
                {"union", UNION},
                {"list", LIST},
                {"void", VOID},
                {"any", ANY},
            },
        };

        std::vector<Type> subTypes{};
        CoreType mainType{};

    public:
        static auto fromNode(ccl::parser::ast::Node *node) -> Type;

        Type() = default;

        Type(CoreType main_type, std::vector<Type> sub_types = {})
          : subTypes{std::move(sub_types)}
          , mainType{main_type}
        {}

        auto operator==(const Type &other) const -> bool;
        auto operator<=>(const Type &other) const -> std::weak_ordering;

        [[nodiscard]] auto isTemplate() const noexcept -> bool
        {
            return !subTypes.empty();
        }

        [[nodiscard]] auto getMainType() const noexcept -> CoreType
        {
            return mainType;
        }

        [[nodiscard]] auto getSubTypes() const noexcept -> const std::vector<Type> &
        {
            return subTypes;
        }

        [[nodiscard]] auto toString() const -> std::string;
    };

}// namespace astlang::interpreter


template<>
struct fmt::formatter<astlang::interpreter::Type> : public fmt::formatter<std::string_view>
{
    auto format(astlang::interpreter::Type type, format_context &ctx) const -> decltype(ctx.out())
    {
        return formatter<std::string_view>::format(type.toString(), ctx);
    }
};

#endif /* CCL_PROJECT_ASTLANG_INTERPRETER_TYPE_HPP */
