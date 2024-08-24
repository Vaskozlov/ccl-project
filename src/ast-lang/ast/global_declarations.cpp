#include <ast-lang/ast/global_declarations.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast
{
    using namespace astlang::interpreter;

    auto GlobalDeclarations::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        for (const auto &node : nodes) {
            ConstNodePtr{node.get()}.astlangNode->compute(interpreter);
        }

        return EvaluationResult{.value = std::nullopt, .type = Type::ANY};
    }
}// namespace astlang::ast