#include <ast-lang/ast/function_decl/return_type.hpp>

namespace astlang::ast::function::decl
{
    using namespace astlang::interpreter;

    auto ReturnType::compute(Interpreter & /* unused */) -> EvaluationResult
    {
        auto return_type_node = NodePtr{this->back()};
        auto type = Type::fromNode(return_type_node.cclNode);

        return EvaluationResult{
            .value = isl::UniqueAny{std::move(type)},
            .type = type,
        };
    }
}// namespace astlang::ast::function::decl