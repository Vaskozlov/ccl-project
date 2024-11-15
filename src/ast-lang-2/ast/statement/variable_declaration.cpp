#include <ast-lang-2/ast/statement/variable_declaration.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    auto VariableDeclaration::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        const auto *name_node = static_cast<const ccl::parser::ast::Terminal *>(at(1).get());
        const ccl::lexer::Token &name_token = name_node->getToken();
        const auto name_repr = name_token.getRepr();

        const auto *value_node = static_cast<const AstlangNode *>(back().get());

        Value value = value_node->compute(interpreter).value;
        value.valueType = ValueType::L_VALUE;

        interpreter.write(static_cast<std::string>(name_repr), value);

        return core::ComputationResult{
            .value = std::move(value),
        };
    }
}// namespace astlang2::ast::statement
