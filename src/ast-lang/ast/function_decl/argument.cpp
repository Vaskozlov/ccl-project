#include <ast-lang/ast/function_decl/argument.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::function::decl
{
    using namespace astlang::interpreter;

    auto Argument::compute(Interpreter &/* unused */) const -> EvaluationResult
    {
        const auto front_node = ConstNodePtr{front().get()};
        const auto name_node = ConstNodePtr{back().get()};

        auto type = Type::fromNode(front_node.cclNode);
        const auto &name_token = name_node.terminalNode->getToken();
        auto argument_name = name_token.getRepr();

        return EvaluationResult{
            isl::makeAny<isl::string_view>(argument_name),
            type,
        };
    }
}// namespace astlang::ast::function::decl