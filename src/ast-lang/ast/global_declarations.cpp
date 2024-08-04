#include <ast-lang/ast/global_declarations.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast
{
    using namespace astlang::interpreter;

    auto GlobalDeclarations::compute(Interpreter &interpreter) -> EvaluationResult
    {
        for (auto &node : getNodes()) {
            NodePtr{node.get()}.astlangNode->compute(interpreter);
        }

        return EvaluationResult{.value = std::nullopt, .type = Type::ANY};
    }
}// namespace astlang::ast