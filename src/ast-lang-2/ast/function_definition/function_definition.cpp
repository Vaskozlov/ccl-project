#include <ast-lang-2/ast/function_definition/function_definition.hpp>
#include <ast-lang-2/function/astlang_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::function::def
{
    auto FunctionDefinition::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        std::size_t elements_offset = 0;

        const auto *function_name_node =
            static_cast<const ccl::parser::ast::Terminal *>(at(1).get());

        const auto *arguments_node = dynamic_cast<const AstlangNode *>(at(3).get());

        if (arguments_node == nullptr) {
            elements_offset += 1;
        }

        const auto *return_type_node =
            dynamic_cast<const AstlangNode *>(at(5 - elements_offset).get());

        if (return_type_node == nullptr) {
            elements_offset += 1;
        }

        const auto *statements = static_cast<const AstlangNode *>(at(7 - elements_offset).get());

        const ccl::lexer::Token &function_name_token = function_name_node->getToken();
        const auto function_name_repr = function_name_token.getRepr();

        std::vector<ts::Type *> function_arguments_type;
        std::vector<std::string> function_argument_name;

        if (arguments_node != nullptr) {
            auto arguments_values = arguments_node->compute(interpreter).value;
            auto *args = static_cast<std::vector<std::pair<ts::Type *, isl::string_view>> *>(
                arguments_values.object.get());

            for (const auto &[arg_type, arg_name] : *args) {
                function_argument_name.emplace_back(arg_name);
                function_arguments_type.emplace_back(arg_type);
            }
        }

        ts::Type *return_type = interpreter.getAny();

        if (return_type_node != nullptr) {
            const auto *return_type_node_value =
                static_cast<const AstlangNode *>(return_type_node->back().get());

            return_type = interpreter.getTypeFromNode(return_type_node_value);
        }

        const astlang2::function::FunctionIdentification function_identification{
            .name = static_cast<std::string>(function_name_repr),
            .arguments = std::move(function_arguments_type),
            .returnType = return_type,
        };

        interpreter.addFunction(
            function_identification,
            std::make_unique<astlang2::function::AstlangFunction>(
                std::move(function_argument_name), statements));

        return core::ComputationResult{
            .value =
                Value{
                    .object = nullptr,
                    .type = nullptr,
                },
        };
    }
}// namespace astlang2::ast::function::def