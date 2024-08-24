#include <ast-lang/ast/statements/variable_declaration.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement
{
    using namespace astlang::interpreter;

    auto VariableDeclaration::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        auto name_node = ConstNodePtr{at(1).get()};
        auto expression_node = ConstNodePtr{this->back().get()};

        const auto &name_token = name_node.tokenNode->getToken();
        const auto variable_name = name_token.getRepr();
        auto value = expression_node.astlangNode->compute(interpreter);

        interpreter.createVariable(std::string{variable_name}, std::move(value));

        return EvaluationResult{
            .value = std::nullopt,
            .type = interpreter::Type::VOID,
        };
    }
}// namespace astlang::ast::statement