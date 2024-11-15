#include <ast-lang-2/ast/function_definition/definition_argument.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::function::def
{
    auto ArgumentDefinition::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        const auto *type_node = static_cast<const AstlangNode *>(front().get());
        const auto *name_node = static_cast<const ccl::parser::ast::Terminal *>(back().get());

        const ccl::lexer::Token &name_token = name_node->getToken();
        const auto name_repr = name_token.getRepr();

        auto *argument_type = interpreter.getTypeFromNode(type_node);

        std::shared_ptr<void> argument =
            std::make_shared<std::pair<ts::Type *, isl::string_view>>(argument_type, name_repr);

        return core::ComputationResult{
            .value =
                Value{
                    .object = std::move(argument),
                    .type = interpreter.getBuiltinType(),
                },
        };
    }
}// namespace astlang2::ast::function::def