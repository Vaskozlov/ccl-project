#include <ast-lang/ast/statements/type_variable_declaration.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement
{
    using namespace astlang::interpreter;

    auto TypeVariableDeclaration::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        const auto name_node = ConstNodePtr{at(1).get()};
        const auto type_node = ConstNodePtr{at(3).get()};
        const auto expression_node = ConstNodePtr{back().get()};

        const auto &name_token = name_node.terminalNode->getToken();
        const auto variable_name = name_token.getRepr();
        auto value = expression_node.astlangNode->compute(interpreter);
        auto expected_type = interpreter.typeSystem.getType(type_node.cclNode);

        // if (expected_type != value.type) {
            // throw std::runtime_error{
                // fmt::format(
                    // "Expected type is {}, but expression resulted into {}", expected_type,
                    // value.type),
            // };
        // }

        // interpreter.createVariable(std::string{variable_name}, std::move(value));
        return EvaluationResult{};
    }
}// namespace astlang::ast::statement