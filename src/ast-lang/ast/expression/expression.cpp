#include <ast-lang/ast/expression/expression.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace astlang::interpreter;

    auto Expression::compute(Interpreter &interpreter) -> EvaluationResult
    {
        auto front_node = NodePtr{this->front()};

        if (this->size() == 1) {
            return front_node.astlangNode->compute(interpreter);
        }

        auto middle_node = NodePtr{this->at(1)};
        auto back_node = NodePtr{this->back()};
        auto arguments = FunctionCallArguments{};
        arguments.reserve(2);

        arguments.emplace_back(front_node.astlangNode->compute(interpreter));
        arguments.emplace_back(back_node.astlangNode->compute(interpreter));

        if (middle_node.cclNode->getType() == interpreter.getRuleId("\'+\'")) {
            return interpreter.call("__addition__", std::move(arguments));
        }

        if (middle_node.cclNode->getType() == interpreter.getRuleId("\'-\'")) {
            return interpreter.call("__subtraction__", std::move(arguments));
        }

        throw std::runtime_error("Invalid term expression");
    }
}// namespace astlang::ast::expression