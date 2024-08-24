#include <ast-lang/ast/function_decl/argument.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::function::decl
{
    using namespace astlang::interpreter;

    auto Argument::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        auto front_node = ConstNodePtr{this->front().get()};
        auto name_node = ConstNodePtr{this->back().get()};

        auto type = Type::fromNode(front_node.cclNode);
        auto &name_token = name_node.tokenNode->getToken();
        auto argument_name = name_token.getRepr();

        return EvaluationResult{
            isl::makeAny<isl::string_view>(argument_name),
            type,
        };
    }
}// namespace astlang::ast::function::decl