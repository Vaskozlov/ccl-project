#include <ast-lang-2/ast/core/node.hpp>
#include <ast-lang-2/ast/expression/binary_expression.hpp>
#include <ast-lang-2/ast/expression/factor.hpp>
#include <ast-lang-2/ast/expression/value.hpp>
#include <ast-lang-2/ast/function_call/call_arguments.hpp>
#include <ast-lang-2/ast/function_call/function_call.hpp>
#include <ast-lang-2/ast/function_call/method_call.hpp>
#include <ast-lang-2/ast/function_definition/definition_argument.hpp>
#include <ast-lang-2/ast/function_definition/definition_arguments.hpp>
#include <ast-lang-2/ast/function_definition/function_definition.hpp>
#include <ast-lang-2/ast/statement/elif_block.hpp>
#include <ast-lang-2/ast/statement/else_block.hpp>
#include <ast-lang-2/ast/statement/for_loop.hpp>
#include <ast-lang-2/ast/statement/if_block.hpp>
#include <ast-lang-2/ast/statement/if_statement.hpp>
#include <ast-lang-2/ast/statement/return_statement.hpp>
#include <ast-lang-2/ast/statement/statement.hpp>
#include <ast-lang-2/ast/statement/statements.hpp>
#include <ast-lang-2/ast/statement/type_variable_declaration.hpp>
#include <ast-lang-2/ast/statement/variable_declaration.hpp>
#include <ast-lang-2/ast/statement/while_loop.hpp>

namespace astlang2::ast::core
{
    auto AstlangNode::optimize() -> SharedNode<>
    {
        runRecursiveOptimization();
        return nullptr;
    }

    auto AstlangNode::runRecursiveOptimization() -> void
    {
        for (auto &node : getNodes()) {
            auto *ast_node = dynamic_cast<AstlangNode *>(node.get());

            if (ast_node == nullptr) {
                continue;
            }

            auto optimized_version = ast_node->optimize();

            if (optimized_version == nullptr) {
                continue;
            }

            node = std::move(optimized_version);
        }
    }

    template<typename T>
    static auto getReconstructPtr() -> void (*)(ccl::parser::ast::NonTerminal *)
    {
        return AstlangNode::reconstructNode<T>;
    }

    auto AstlangNode::buildConversionTable(const ccl::parser::reader::ParserBuilder &constructor)
        -> ConversionTable
    {
        return ConversionTable{
            {
                constructor.getRuleId("GLOBAL_DECLARATIONS"),
                getReconstructPtr<statement::Statements>(),
            },
            {
                constructor.getRuleId("GLOBAL_DECLARATION"),
                getReconstructPtr<statement::Statement>(),
            },
            {
                constructor.getRuleId("FUNCTION_DEFINITION"),
                getReconstructPtr<function::def::FunctionDefinition>(),
            },
            {
                constructor.getRuleId("FUNCTION_DEFINITION_ARGUMENTS"),
                getReconstructPtr<function::def::ArgumentsDefinition>(),
            },
            {
                constructor.getRuleId("FUNCTION_DEFINITION_ARGUMENT"),
                getReconstructPtr<function::def::ArgumentDefinition>(),
            },
            {
                constructor.getRuleId("FUNCTION_RETURN_TYPE"),
                getReconstructPtr<AstlangNode>(),
            },
            {
                constructor.getRuleId("STATEMENTS"),
                getReconstructPtr<statement::Statements>(),
            },
            {
                constructor.getRuleId("STATEMENT"),
                getReconstructPtr<statement::Statement>(),
            },
            {
                constructor.getRuleId("IF_STATEMENT"),
                getReconstructPtr<statement::IfStatement>(),
            },
            {
                constructor.getRuleId("IF_BLOCK"),
                getReconstructPtr<statement::IfBlock>(),
            },
            {
                constructor.getRuleId("ELIF_BLOCK"),
                getReconstructPtr<statement::ElifBlock>(),
            },
            {
                constructor.getRuleId("ELSE_BLOCK"),
                getReconstructPtr<statement::ElseBlock>(),
            },
            {
                constructor.getRuleId("VARIABLE_DECLARATION"),
                getReconstructPtr<statement::VariableDeclaration>(),
            },
            {
                constructor.getRuleId("TYPE_VARIABLE_DECLARATION"),
                getReconstructPtr<statement::TypeVariableDeclaration>(),
            },
            {
                constructor.getRuleId("FOR_LOOP"),
                getReconstructPtr<statement::ForLoop>(),
            },
            {
                constructor.getRuleId("WHILE_LOOP"),
                getReconstructPtr<statement::WhileLoop>(),
            },
            {
                constructor.getRuleId("RETURN_STATEMENT"),
                getReconstructPtr<statement::ReturnStatement>(),
            },
            {
                constructor.getRuleId("ASSIGNMENT_EXPRESSION"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("LOGICAL_OR_EXPRESSION"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("LOGICAL_AND_EXPRESSION"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("BITWISE_OR_EXPRESSION"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("BITWISE_XOR_EXPRESSION"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("BITWISE_AND_EXPRESSION"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("EQUALITY"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("COMPARISON"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("EXPRESSION"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("TERM"),
                getReconstructPtr<expression::BinaryExpression>(),
            },
            {
                constructor.getRuleId("VALUE"),
                getReconstructPtr<expression::Value>(),
            },
            {
                constructor.getRuleId("FACTOR"),
                getReconstructPtr<expression::Factor>(),
            },
            {
                constructor.getRuleId("FUNCTION_CALL"),
                getReconstructPtr<function::call::FunctionCall>(),
            },
            {
                constructor.getRuleId("FUNCTION_CALL_ARGUMENTS"),
                getReconstructPtr<function::call::CallArguments>(),
            },
            {
                constructor.getRuleId("METHOD_CALL"),
                getReconstructPtr<function::call::MethodCall>(),
            },
            {
                constructor.getRuleId("TYPE"),
                getReconstructPtr<AstlangNode>(),
            },
        };
    }
}// namespace astlang2::ast::core
