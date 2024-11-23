#include <ast-lang-2/ast/expression/binary_expression.hpp>
#include <ast-lang-2/ast/expression/factor.hpp>
#include <ast-lang-2/ast/expression/field_access.hpp>
#include <ast-lang-2/ast/expression/value.hpp>
#include <ast-lang-2/ast/function_call/function_call.hpp>
#include <ast-lang-2/ast/function_call/method_call.hpp>
#include <ast-lang-2/ast/function_definition/function_definition.hpp>
#include <ast-lang-2/ast/node.hpp>
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

namespace astlang2::ast
{
    auto AstlangNode::print(
        const std::string &prefix, const bool is_left,
        const std::function<std::string(ccl::SmallId)> &id_converter) const -> void
    {
        const auto expanded_prefix = expandPrefix(prefix, is_left);
        std::cout << fmt::format(
                         "{}NodeSequence-{}", getPrintingPrefix(prefix, is_left),
                         id_converter(getType()))
                  << std::endl;

        auto generator = getChildrenNodes();
        SharedNode<> next = generator();

        while (next != nullptr) {
            SharedNode<> node = std::move(next);
            next = generator();

            node->print(expanded_prefix, next != nullptr, id_converter);
        }
    }

    auto AstlangNode::buildConversionTable(const ccl::parser::reader::ParserBuilder &constructor)
        -> ConversionTable
    {
        return ConversionTable{
            {
                constructor.getRuleId("GLOBAL_DECLARATIONS"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::Statements>,
            },
            {
                constructor.getRuleId("GLOBAL_DECLARATION"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::Statement>,
            },
            {
                constructor.getRuleId("FUNCTION_DEFINITION"),
                ccl::parser::ast::NonTerminal::reconstructNode<function::def::FunctionDefinition>,
            },
            {
                constructor.getRuleId("FUNCTION_RETURN_TYPE"),
                ccl::parser::ast::NonTerminal::reconstructNode<ccl::parser::ast::NonTerminal>,
            },
            {
                constructor.getRuleId("STATEMENTS"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::Statements>,
            },
            {
                constructor.getRuleId("STATEMENT"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::Statement>,
            },
            {
                constructor.getRuleId("IF_STATEMENT"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::IfStatement>,
            },
            {
                constructor.getRuleId("IF_BLOCK"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::IfBlock>,
            },
            {
                constructor.getRuleId("ELIF_BLOCK"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::ElifBlock>,
            },
            {
                constructor.getRuleId("ELSE_BLOCK"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::ElseBlock>,
            },
            {
                constructor.getRuleId("VARIABLE_DECLARATION"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::VariableDeclaration>,
            },
            {
                constructor.getRuleId("TYPE_VARIABLE_DECLARATION"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::TypeVariableDeclaration>,
            },
            {
                constructor.getRuleId("FOR_LOOP"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::ForLoop>,
            },
            {
                constructor.getRuleId("WHILE_LOOP"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::WhileLoop>,
            },
            {
                constructor.getRuleId("RETURN_STATEMENT"),
                ccl::parser::ast::NonTerminal::reconstructNode<statement::ReturnStatement>,
            },
            {
                constructor.getRuleId("ASSIGNMENT_EXPRESSION"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("LOGICAL_OR_EXPRESSION"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("LOGICAL_AND_EXPRESSION"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("BITWISE_OR_EXPRESSION"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("BITWISE_XOR_EXPRESSION"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("BITWISE_AND_EXPRESSION"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("EQUALITY"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("COMPARISON"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("EXPRESSION"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("TERM"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::BinaryExpression>,
            },
            {
                constructor.getRuleId("VALUE"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::Value>,
            },
            {
                constructor.getRuleId("FACTOR"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::Factor>,
            },
            {
                constructor.getRuleId("FUNCTION_CALL"),
                ccl::parser::ast::NonTerminal::reconstructNode<function::call::FunctionCall>,
            },
            {
                constructor.getRuleId("METHOD_CALL"),
                ccl::parser::ast::NonTerminal::reconstructNode<function::call::MethodCall>,
            },
            {
                constructor.getRuleId("FIELD_ACCESS"),
                ccl::parser::ast::NonTerminal::reconstructNode<expression::FieldAccess>,
            },
        };
    }
}// namespace astlang2::ast
