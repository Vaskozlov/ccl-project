#include <ast-lang-2/ast/statement/type_variable_declaration.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    TypeVariableDeclaration::TypeVariableDeclaration(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , typeNode{isl::staticPointerCast<ccl::parser::ast::NonTerminal>(initial_nodes.at(3))}
      , initialValueNode{isl::staticPointerCast<AstlangNode>(initial_nodes.back())}
    {
        const auto *name_node =
            static_cast<const ccl::parser::ast::Terminal *>(initial_nodes.at(1).get());

        const ccl::lexer::Token &name_token = name_node->getToken();
        const auto name_repr = name_token.getRepr();

        variableName = static_cast<std::string>(name_repr);
    }

    auto TypeVariableDeclaration::compute(interpreter::Interpreter &interpreter) const
        -> ComputationResult
    {
        const ts::Type *declaration_type = interpreter.getTypeFromNode(typeNode.get());

        Value value = initialValueNode->compute(interpreter).value;
        value.valueType = ValueType::L_VALUE;

        if (value.type != declaration_type) {
            throw std::runtime_error{fmt::format("Declaration type mismatch")};
        }

        interpreter.write(variableName, value);

        return ComputationResult{
            .value = std::move(value),
        };
    }

    auto TypeVariableDeclaration::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(initialValueNode, initialValueNode->optimize());

        return nullptr;
    }

    auto TypeVariableDeclaration::getChildrenNodes() const
        -> ChildrenNodesGenerator
    {
        return ChildrenNodesGenerator{
            [this, field_index = 0U]() mutable -> ccl::parser::ast::SharedNode<> {
                switch (field_index++) {
                case 0:
                    return typeNode;

                case 1:
                    return initialValueNode;

                default:
                    return nullptr;
                }
            }};
    }

}// namespace astlang2::ast::statement
