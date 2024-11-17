#include <ast-lang-2/ast/core/node.hpp>
#include <ast-lang-2/ast/expression/binary_expression.hpp>
#include <ast-lang-2/ast/expression/factor.hpp>
#include <ast-lang-2/ast/expression/value.hpp>
#include <ast-lang-2/ast/function_call/function_call.hpp>
#include <ast-lang-2/ast/function_call/method_call.hpp>
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
    template<typename T>
    void (*ReconstructorPtr)(ccl::parser::ast::NonTerminal *) =
        ccl::parser::ast::NonTerminal::reconstructNode<T>;

    auto AstlangNode::buildConversionTable(const ccl::parser::reader::ParserBuilder &constructor)
        -> ConversionTable
    {
        return ConversionTable{
            {
                constructor.getRuleId("GLOBAL_DECLARATIONS"),
                ReconstructorPtr<statement::Statements>,
            },
            {
                constructor.getRuleId("GLOBAL_DECLARATION"),
                ReconstructorPtr<statement::Statement>,
            },
            {
                constructor.getRuleId("FUNCTION_DEFINITION"),
                ReconstructorPtr<function::def::FunctionDefinition>,
            },
            {
                constructor.getRuleId("FUNCTION_RETURN_TYPE"),
                ReconstructorPtr<ccl::parser::ast::NonTerminal>,
            },
            {
                constructor.getRuleId("STATEMENTS"),
                ReconstructorPtr<statement::Statements>,
            },
            {
                constructor.getRuleId("STATEMENT"),
                ReconstructorPtr<statement::Statement>,
            },
            {
                constructor.getRuleId("IF_STATEMENT"),
                ReconstructorPtr<statement::IfStatement>,
            },
            {
                constructor.getRuleId("IF_BLOCK"),
                ReconstructorPtr<statement::IfBlock>,
            },
            {
                constructor.getRuleId("ELIF_BLOCK"),
                ReconstructorPtr<statement::ElifBlock>,
            },
            {
                constructor.getRuleId("ELSE_BLOCK"),
                ReconstructorPtr<statement::ElseBlock>,
            },
            {
                constructor.getRuleId("VARIABLE_DECLARATION"),
                ReconstructorPtr<statement::VariableDeclaration>,
            },
            {
                constructor.getRuleId("TYPE_VARIABLE_DECLARATION"),
                ReconstructorPtr<statement::TypeVariableDeclaration>,
            },
            {
                constructor.getRuleId("FOR_LOOP"),
                ReconstructorPtr<statement::ForLoop>,
            },
            {
                constructor.getRuleId("WHILE_LOOP"),
                ReconstructorPtr<statement::WhileLoop>,
            },
            {
                constructor.getRuleId("RETURN_STATEMENT"),
                ReconstructorPtr<statement::ReturnStatement>,
            },
            {
                constructor.getRuleId("ASSIGNMENT_EXPRESSION"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("LOGICAL_OR_EXPRESSION"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("LOGICAL_AND_EXPRESSION"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("BITWISE_OR_EXPRESSION"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("BITWISE_XOR_EXPRESSION"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("BITWISE_AND_EXPRESSION"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("EQUALITY"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("COMPARISON"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("EXPRESSION"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("TERM"),
                ReconstructorPtr<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("VALUE"),
                ReconstructorPtr<expression::Value>,
            },
            {
                constructor.getRuleId("FACTOR"),
                ReconstructorPtr<expression::Factor>,
            },
            {
                constructor.getRuleId("FUNCTION_CALL"),
                ReconstructorPtr<function::call::FunctionCall>,
            },
            {
                constructor.getRuleId("METHOD_CALL"),
                ReconstructorPtr<function::call::MethodCall>,
            },
        };
    }
}// namespace astlang2::ast::core
