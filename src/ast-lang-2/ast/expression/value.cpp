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

    auto Value::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        if (functionName.empty()) {
            return node->compute(interpreter);
        }

        core::ComputationResult expression_argument = node->compute(interpreter);
        const isl::SmallVector<astlang2::Value, 4> function_arguments{
            std::move(expression_argument.value)};

        return core::ComputationResult{
            .value = interpreter.callFunction(functionName, function_arguments),
        };
    }

    auto Value::castChildren(const ConversionTable &conversion_table) -> void
    {
        node->cast(conversion_table);
    }

    auto Value::optimize() -> core::SharedNode<>
    {
        auto new_node = node->optimize();

        if (new_node != nullptr) {
            node = isl::staticPointerCast<AstlangNode>(std::move(new_node));
        }

        if (functionName.empty()) {
            return node;
        }

        return nullptr;
    }
}// namespace astlang2::ast::expression
