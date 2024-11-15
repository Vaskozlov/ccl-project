#include <ast-lang/ast/global_declaration.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast
{
    using namespace astlang::interpreter;

    auto GlobalDeclaration::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        const auto front_node_ptr = ConstNodePtr{front().get()};
        front_node_ptr.astlangNode->compute(interpreter);

        return interpreter::EvaluationResult{
            .instance = Instance{},
            .needToReturn = false,
            .storesReference = false,
        };
    }
}// namespace astlang::ast