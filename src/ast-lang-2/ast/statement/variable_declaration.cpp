#include <ast-lang-2/ast/statement/variable_declaration.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement {
    VariableDeclaration::VariableDeclaration(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes&initial_nodes)
        : AstlangNode{id}
          , initialValueNode{isl::staticPointerCast<AstlangNode>(initial_nodes.back())} {
        const auto* name_node =
                static_cast<const ccl::parser::ast::Terminal *>(initial_nodes.at(1).get());

        const ccl::lexer::Token&name_token = name_node->getToken();
        const auto name_repr = name_token.getRepr();

        variableName = static_cast<std::string>(name_repr);
    }


    auto VariableDeclaration::compute(interpreter::Interpreter&interpreter) const
        -> core::ComputationResult {
        Value value = initialValueNode->compute(interpreter).value;
        value.valueType = ValueType::L_VALUE;

        interpreter.write(variableName, value);

        return core::ComputationResult{
            .value = std::move(value),
        };
    }

    auto VariableDeclaration::castChildren(const ConversionTable&conversion_table) -> void {
        initialValueNode->cast(conversion_table);
    }

    auto VariableDeclaration::optimize() -> core::SharedNode<> {
        auto new_initial_value = initialValueNode->optimize();

        if (new_initial_value != nullptr) {
            initialValueNode = isl::staticPointerCast<AstlangNode>(std::move(new_initial_value));
        }

        return nullptr;
    }
} // namespace astlang2::ast::statement
