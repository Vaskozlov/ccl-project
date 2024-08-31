#include <ast-lang/ast/function_decl/return_type.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::function::decl
{
    using namespace astlang::interpreter;

    auto ReturnType::compute(Interpreter & /* unused */) const -> EvaluationResult
    {
        const auto return_type_node = ConstNodePtr{back().get()};
        auto type = Type::fromNode(return_type_node.cclNode);

        return EvaluationResult{
            .value = isl::UniqueAny{std::move(type)},
            .type = type,
        };
    }
}// namespace astlang::ast::function::decl
