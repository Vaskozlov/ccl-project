#include <ast-lang-2/ast/expression/value.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::expression
{
    auto Value::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        if (size() == 1) {
            return static_cast<AstlangNode *>(front().get())->compute(interpreter);
        }

        const auto *front_node = static_cast<ccl::parser::ast::Terminal *>(front().get());
        const auto node_type = front_node->getType();
        std::string function_name;

        if (node_type == interpreter.PLUS) {
            function_name = "__positive__";
        } else if (node_type == interpreter.MINUS) {
            function_name = "__negation__";
        } else {
            throw std::runtime_error("Invalid value expression");
        }

        const auto *back_node = static_cast<AstlangNode *>(back().get());
        core::ComputationResult expression_argument = back_node->compute(interpreter);
        const std::vector<astlang2::Value> function_arguments{std::move(expression_argument.value)};

        return core::ComputationResult{
            .value = interpreter.callFunction(function_name, function_arguments),
        };
    }
}// namespace astlang2::ast::expression
