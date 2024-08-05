#include <ast-lang/ast/expression/value.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    auto Value::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        auto front_node = ConstNodePtr{this->front()};

        if (this->size() == 1) {
            return front_node.astlangNode->compute(interpreter);
        }

        auto back_node = ConstNodePtr{this->back()};
        auto arguments = interpreter::FunctionCallArguments{};
        arguments.reserve(1);

        arguments.emplace_back(back_node.astlangNode->compute(interpreter));

        if (front_node.cclNode->getType() == interpreter.getRuleId("\"+\"")) {
            return interpreter.call("__positive__", std::move(arguments));
        }

        if (front_node.cclNode->getType() == interpreter.getRuleId("\"-\"")) {
            return interpreter.call("__negation__", std::move(arguments));
        }

        throw std::runtime_error("Invalid value expression");
    }
}// namespace astlang::ast::expression