#include <ast-lang-2/ast/expression/value.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::expression
{
    Value::Value(const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &nodes)
      : AstlangNode{id}
    {
        if (nodes.size() == 1) {
            node = isl::staticPointerCast<AstlangNode>(nodes.front());
            return;
        }

        node = isl::staticPointerCast<AstlangNode>(nodes.at(1));

        switch (const auto *front_node = nodes.front().get();
                static_cast<NodeTypes>(front_node->getType())) {
            using enum NodeTypes;

        case PLUS:
            functionName = "__positive__";
            break;

        case MINUS:
            functionName = "__negation__";
            break;

        default:
            throw std::runtime_error{"Unsupported operation"};
        }
    }

    auto Value::compute(interpreter::Interpreter &interpreter) const -> ComputationResult
    {
        if (functionName.empty()) {
            return node->compute(interpreter);
        }

        ComputationResult expression_argument = node->compute(interpreter);
        const isl::SmallVector<astlang2::Value, 4> function_arguments{
            std::move(expression_argument.value)};

        return ComputationResult{
            .value = interpreter.callFunction(functionName, function_arguments),
        };
    }

    auto Value::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(node, node->optimize());

        if (functionName.empty()) {
            return node;
        }

        return nullptr;
    }

    auto Value::getChildrenNodes() const -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()>
    {
        return isl::SmallFunction<ccl::parser::ast::SharedNode<>()>{
            [index = 0, stored_node = node]() mutable -> ccl::parser::ast::SharedNode<> {
                if (index > 0) {
                    return nullptr;
                }

                ++index;
                return stored_node;
            }};
    }

}// namespace astlang2::ast::expression
