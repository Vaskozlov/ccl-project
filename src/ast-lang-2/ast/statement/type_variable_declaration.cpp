#include <ast-lang-2/ast/statement/type_variable_declaration.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    auto TypeVariableDeclaration::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        const auto *name_node = static_cast<const ccl::parser::ast::Terminal *>(at(1).get());
        const ccl::lexer::Token &name_token = name_node->getToken();
        const auto name_repr = name_token.getRepr();

        const auto *type_node = static_cast<const AstlangNode *>(at(3).get());
        const ts::Type *declaration_type = interpreter.getTypeFromNode(type_node);

        const auto *value_node = static_cast<const AstlangNode *>(back().get());

        Value value = value_node->compute(interpreter).value;
        value.valueType = ValueType::L_VALUE;

        if (value.type != declaration_type) {
            throw std::runtime_error{fmt::format("Declaration type mismatch")};
        }

        interpreter.write(static_cast<std::string>(name_repr), value);

        return core::ComputationResult{
            .value = std::move(value),
        };
    }
}// namespace astlang2::ast::statement
