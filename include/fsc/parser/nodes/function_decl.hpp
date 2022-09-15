#ifndef CCL_PROJECT_FUNCTION_DECL_HPP
#define CCL_PROJECT_FUNCTION_DECL_HPP

#include <fsc/parser/nodes/basic.hpp>
#include <fsc/parser/nodes/params_list.hpp>

namespace fsc::ast
{
    struct FunctionDeclarationNode : BasicNode
    {
        FunctionDeclarationNode() = default;

        CCL_PERFECT_FORWARDING_2(T1, Token, T2, Token)
        explicit FunctionDeclarationNode(
            T1 &&name_, T2 &&return_type_, unique_ptr<ParamsListNode> parameters_ = nullptr)
          : name{ std::forward<T1>(name_) }, return_type{ std::forward<T2>(return_type_) },
            parameters{ std::move(parameters_) }
        {}

        auto print(const std::string &prefix, bool isLeft) const -> void override
        {
            fmt::print(
                "{}{}Function: {} -> {}\n", prefix,
                isLeft ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500", name.getRepr(),
                return_type.getRepr());

            if (parameters) {
                parameters->print(prefix + (isLeft ? "\u2502  " : "   "), false);
            }
        }

        Token name;
        Token return_type;
        unique_ptr<ParamsListNode> parameters;
        unique_ptr<BasicNode> body;
    };
}// namespace fsc::ast

#endif /* CCL_PROJECT_FUNCTION_DECL_HPP */
