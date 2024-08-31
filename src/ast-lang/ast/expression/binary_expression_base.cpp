#include <ast-lang/ast/expression/binary_expression_base.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto BinaryExpressionBase::computeBinaryExpression(
        Interpreter &interpreter,
        const std::vector<std::pair<SmallId, std::string>> &token_to_function_name) const
        -> interpreter::EvaluationResult
    {
        const auto front_node = ConstNodePtr{front().get()};

        if (size() == 1) {
            return front_node.astlangNode->compute(interpreter);
        }

        const auto *middle_node = at(1).get();
        const auto back_node = ConstNodePtr{back().get()};
        auto arguments = FunctionCallArguments{};
        arguments.reserve(2);

        arguments.emplace_back(front_node.astlangNode->compute(interpreter));
        arguments.emplace_back(back_node.astlangNode->compute(interpreter));

        auto it = std::ranges::find_if(
            token_to_function_name, [type = middle_node->getType()](const auto &pair) {
                return pair.first == type;
            });

        if (it == token_to_function_name.end()) {
            throw std::runtime_error("Invalid binary expression");
        }

        return interpreter.call(it->second, std::move(arguments));
    }
}// namespace astlang::ast::expression
