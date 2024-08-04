#include <ast-lang/ast/global_declaration.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast
{
    using namespace astlang::interpreter;

    auto GlobalDeclaration::compute(Interpreter &interpreter) -> EvaluationResult
    {
        auto front_node_ptr = NodePtr{this->front()};
        front_node_ptr.astlangNode->compute(interpreter);

        return EvaluationResult{
            .value = std::nullopt,
            .type = Type::ANY,
        };
    }
}// namespace astlang::ast