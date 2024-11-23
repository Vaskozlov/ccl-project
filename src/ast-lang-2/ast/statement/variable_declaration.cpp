#include <ast-lang-2/ast/statement/variable_declaration.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    VariableDeclaration::VariableDeclaration(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , initialValueNode{isl::staticPointerCast<AstlangNode>(initial_nodes.back())}
    {
        const auto *name_node =
            static_cast<const ccl::parser::ast::Terminal *>(initial_nodes.at(1).get());

        const ccl::lexer::Token &name_token = name_node->getToken();
        const auto name_repr = name_token.getRepr();

        variableName = static_cast<std::string>(name_repr);
    }


    auto VariableDeclaration::compute(interpreter::Interpreter &interpreter) const
        -> ComputationResult
    {
        Value value = initialValueNode->compute(interpreter).value;
        value.valueType = ValueType::L_VALUE;

        interpreter.write(variableName, value);

        return ComputationResult{
            .value = std::move(value),
        };
    }

    auto VariableDeclaration::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(initialValueNode, initialValueNode->optimize());
        return nullptr;
    }

    auto VariableDeclaration::getChildrenNodes() const
        -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()>
    {
        return isl::SmallFunction<ccl::parser::ast::SharedNode<>()>{
            [index = 0,
             stored_node = initialValueNode]() mutable -> ccl::parser::ast::SharedNode<> {
                if (index > 0) {
                    return nullptr;
                }

                ++index;
                return stored_node;
            }};
    }
}// namespace astlang2::ast::statement
