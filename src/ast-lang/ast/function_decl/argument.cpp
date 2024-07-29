#include <ast-lang/ast/function_decl/argument.hpp>

namespace astlang::ast::function::decl
{
    using namespace astlang::interpreter;

    auto Argument::compute(Interpreter &interpreter) -> EvaluationResult
    {
        auto front_node = NodePtr{this->front()};
        auto name_node = NodePtr{this->back()};

        auto type = Type::fromNode(front_node.cclNode);
        auto &name_token = name_node.tokenNode->getToken();
        auto argument_name = name_token.getRepr();

        return EvaluationResult{
            isl::makeAny<isl::string_view>(argument_name),
            type,
        };
    }
}// namespace astlang::ast::function::decl