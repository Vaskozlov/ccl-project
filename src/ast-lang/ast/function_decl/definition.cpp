#include <ast-lang/ast/function_decl/declaration.hpp>
#include <ast-lang/interpreter/astlang_function.hpp>

namespace astlang::ast::function::decl
{
    using namespace astlang::interpreter;

    constexpr static std::size_t FUNCTION_NAME_INDEX = 1;
    constexpr static std::size_t FUNCTION_ARGUMENTS_INDEX = 3;
    constexpr static std::size_t FUNCTION_RETURN_TYPE_INDEX = 5;
    constexpr static std::size_t FUNCTION_BODY_INDEX_IN_CASE_OF_IMPLICIT_RETURN_TYPE = 6;
    constexpr static std::size_t FUNCTION_BODY_INDEX_IN_CASE_OF_EXPLICIT_RETURN_TYPE = 7;

    constexpr static std::size_t SIZE_IN_CASE_IMPLICIT_RETURN_TYPE = 8;

    auto Definition::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        const auto function_name_node = ConstNodePtr{at(FUNCTION_NAME_INDEX).get()};
        const auto function_arguments_node = ConstNodePtr{at(FUNCTION_ARGUMENTS_INDEX).get()};

        const auto &function_name_token = function_name_node.terminalNode->getToken();
        const auto function_name = function_name_token.getRepr();

        auto function_arguments = function_arguments_node.astlangNode->compute(interpreter);
        auto arguments_list = isl::get<std::vector<EvaluationResult>>(function_arguments.value);
        auto function_definition_arguments_type = FunctionArguments{};
        auto function_definition_arguments_names = std::vector<std::string>{};
        auto function_body = ConstNodePtr{nullptr};
        auto return_type = Type{Type::ANY};

        if (size() == SIZE_IN_CASE_IMPLICIT_RETURN_TYPE) {
            function_body = ConstNodePtr{
                at(FUNCTION_BODY_INDEX_IN_CASE_OF_IMPLICIT_RETURN_TYPE).get(),
            };
        } else {
            function_body = ConstNodePtr{
                at(FUNCTION_BODY_INDEX_IN_CASE_OF_EXPLICIT_RETURN_TYPE).get(),
            };

            auto return_type_node = ConstNodePtr{at(FUNCTION_RETURN_TYPE_INDEX).get()};
            auto return_type_evaluation = return_type_node.astlangNode->compute(interpreter);

            return_type = std::move(return_type_evaluation.type);
        }

        for (auto &[value, type, need_to_return, stores_reference] : arguments_list) {
            const auto &argument_type = type;
            auto argument_name = isl::get<isl::string_view>(value);

            function_definition_arguments_type.emplace_back(argument_type);
            function_definition_arguments_names.emplace_back(argument_name);
        }

        auto function_identification = FunctionIdentification{
            .name = std::string{function_name},
            .returnType = return_type,
            .parameters = std::move(function_definition_arguments_type),
        };

        auto function_ptr = std::make_unique<AstlangFunction>(
            std::move(function_definition_arguments_names), function_body.astlangNode);

        interpreter.addFunction(std::move(function_identification), std::move(function_ptr));

        return EvaluationResult{
            .value = std::nullopt,
            .type = Type::ANY,
        };
    }
}// namespace astlang::ast::function::decl