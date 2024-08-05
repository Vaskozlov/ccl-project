#include <ast-lang/ast/expression/bitwise_and.hpp>
#include <ast-lang/ast/expression/bitwise_or.hpp>
#include <ast-lang/ast/expression/bitwise_xor.hpp>
#include <ast-lang/ast/expression/comparison.hpp>
#include <ast-lang/ast/expression/equality.hpp>
#include <ast-lang/ast/expression/expression.hpp>
#include <ast-lang/ast/expression/factor.hpp>
#include <ast-lang/ast/expression/logical_and.hpp>
#include <ast-lang/ast/expression/logical_or.hpp>
#include <ast-lang/ast/expression/term.hpp>
#include <ast-lang/ast/expression/value.hpp>
#include <ast-lang/ast/function_call/arguments.hpp>
#include <ast-lang/ast/function_call/function_call.hpp>
#include <ast-lang/ast/function_call/method_call.hpp>
#include <ast-lang/ast/function_decl/argument.hpp>
#include <ast-lang/ast/function_decl/arguments.hpp>
#include <ast-lang/ast/function_decl/declaration.hpp>
#include <ast-lang/ast/function_decl/return_type.hpp>
#include <ast-lang/ast/global_declaration.hpp>
#include <ast-lang/ast/global_declarations.hpp>
#include <ast-lang/ast/goal.hpp>
#include <ast-lang/ast/lexer/lexer_rule.hpp>
#include <ast-lang/ast/lexer/lexer_rule_body.hpp>
#include <ast-lang/ast/node.hpp>
#include <ast-lang/ast/parser/parser_functions.hpp>
#include <ast-lang/ast/parser/parser_rule.hpp>
#include <ast-lang/ast/parser/parser_rule_body.hpp>
#include <ast-lang/ast/parser/parser_symbols.hpp>
#include <ast-lang/ast/statements/for_loop.hpp>
#include <ast-lang/ast/statements/if/elif_block.hpp>
#include <ast-lang/ast/statements/if/else_block.hpp>
#include <ast-lang/ast/statements/if/if_block.hpp>
#include <ast-lang/ast/statements/if/if_statement.hpp>
#include <ast-lang/ast/statements/return_statement.hpp>
#include <ast-lang/ast/statements/statement.hpp>
#include <ast-lang/ast/statements/statements.hpp>
#include <ast-lang/ast/statements/type_variable_declaration.hpp>
#include <ast-lang/ast/statements/variable_declaration.hpp>
#include <ast-lang/ast/type.hpp>
#include <ccl/parser/rules_reader/rules_constructor.hpp>

namespace astlang::ast
{
    using namespace ccl::parser;

    auto Node::optimize() -> ccl::parser::ast::UnNodePtr
    {
        runRecursiveOptimization();
        return nullptr;
    }

    auto Node::runRecursiveOptimization() -> void
    {
        for (auto &node : getNodes()) {
            auto *ast_node = dynamic_cast<Node *>(node.get());

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

    auto Node::convertCclTreeToAstlang(reader::RulesConstructor &constructor, Node *node) -> void
    {
        const auto conversion_table = ConvertionTable{
            {
                constructor.getRuleId("GOAL"),
                reconstructNode<Goal>,
            },
            {
                constructor.getRuleId("GLOBAL_DECLARATIONS"),
                reconstructNode<GlobalDeclarations>,
            },
            {
                constructor.getRuleId("GLOBAL_DECLARATION"),
                reconstructNode<GlobalDeclaration>,
            },
            {
                constructor.getRuleId("LEXER_RULE"),
                reconstructNode<LexerRule>,
            },
            {
                constructor.getRuleId("LEXER_RULE_BODY"),
                reconstructNode<LexerRuleBody>,
            },
            {
                constructor.getRuleId("PARSER_RULE"),
                reconstructNode<ParserRule>,
            },
            {
                constructor.getRuleId("PARSER_RULE_BODY"),
                reconstructNode<ParserRuleBody>,
            },
            {
                constructor.getRuleId("PARSER_FUNCTIONS"),
                reconstructNode<ParserFunctions>,
            },
            {
                constructor.getRuleId("PARSER_SYMBOLS"),
                reconstructNode<ParserSymbols>,
            },
            {
                constructor.getRuleId("FUNCTION_DEFINITION"),
                reconstructNode<function::decl::Definition>,
            },
            {
                constructor.getRuleId("FUNCTION_DEFINITION_ARGUMENTS"),
                reconstructNode<function::decl::Arguments>,
            },
            {
                constructor.getRuleId("FUNCTION_DEFINITION_ARGUMENT"),
                reconstructNode<function::decl::Argument>,
            },
            {
                constructor.getRuleId("FUNCTION_RETURN_TYPE"),
                reconstructNode<function::decl::ReturnType>,
            },
            {
                constructor.getRuleId("STATEMENTS"),
                reconstructNode<statement::Statements>,
            },
            {
                constructor.getRuleId("STATEMENT"),
                reconstructNode<statement::Statement>,
            },
            {
                constructor.getRuleId("IF_STATEMENT"),
                reconstructNode<statement::if_statement::IfStatement>,
            },
            {
                constructor.getRuleId("IF_BLOCK"),
                reconstructNode<statement::if_statement::IfBlock>,
            },
            {
                constructor.getRuleId("ELIF_BLOCK"),
                reconstructNode<statement::if_statement::ElifBlock>,
            },
            {
                constructor.getRuleId("ELSE_BLOCK"),
                reconstructNode<statement::if_statement::ElseBlock>,
            },
            {
                constructor.getRuleId("VARIABLE_DECLARATION"),
                reconstructNode<statement::VariableDeclaration>,
            },
            {
                constructor.getRuleId("TYPE_VARIABLE_DECLARATION"),
                reconstructNode<statement::TypeVariableDeclaration>,
            },
            {
                constructor.getRuleId("FOR_LOOP"),
                reconstructNode<statement::ForLoop>,
            },
            {
                constructor.getRuleId("RETURN_STATEMENT"),
                reconstructNode<statement::ReturnStatement>,
            },
            {
                constructor.getRuleId("LOGICAL_OR_EXPRESSION"),
                reconstructNode<expression::LogicalOr>,
            },
            {
                constructor.getRuleId("LOGICAL_AND_EXPRESSION"),
                reconstructNode<expression::LogicalAnd>,
            },
            {
                constructor.getRuleId("BITWISE_OR_EXPRESSION"),
                reconstructNode<expression::BitwiseOr>,
            },
            {
                constructor.getRuleId("BITWISE_XOR_EXPRESSION"),
                reconstructNode<expression::BitwiseXor>,
            },
            {
                constructor.getRuleId("BITWISE_AND_EXPRESSION"),
                reconstructNode<expression::BitwiseAnd>,
            },
            {
                constructor.getRuleId("EQUALITY"),
                reconstructNode<expression::Equality>,
            },
            {
                constructor.getRuleId("COMPARISON"),
                reconstructNode<expression::Comparison>,
            },
            {
                constructor.getRuleId("EXPRESSION"),
                reconstructNode<expression::Expression>,
            },
            {
                constructor.getRuleId("TERM"),
                reconstructNode<expression::Term>,
            },
            {
                constructor.getRuleId("VALUE"),
                reconstructNode<expression::Value>,
            },
            {
                constructor.getRuleId("FACTOR"),
                reconstructNode<expression::Factor>,
            },
            {
                constructor.getRuleId("FUNCTION_CALL"),
                reconstructNode<function::call::FunctionCall>,
            },
            {
                constructor.getRuleId("FUNCTION_CALL_ARGUMENTS"),
                reconstructNode<function::call::Arguments>,
            },
            {
                constructor.getRuleId("METHOD_CALL"),
                reconstructNode<function::call::MethodCall>,
            },
            {
                constructor.getRuleId("TYPE"),
                reconstructNode<astlang::ast::Type>,
            },
        };

        node->castChildrenToAstLangNode(conversion_table);
    }

    auto Node::castChildrenToAstLangNode(const ConvertionTable &conversion_table) -> void
    {
        auto *self_as_sequence = dynamic_cast<ccl::parser::ast::UnNodeSequence *>(this);

        if (self_as_sequence == nullptr) {
            return;
        }

        auto new_self_nodes = std::vector<isl::UniquePtr<ccl::parser::ast::Node>>{};

        for (auto &node : self_as_sequence->getNodes()) {
            new_self_nodes.emplace_back(castToAstLangNode(conversion_table, std::move(node)));
        }

        self_as_sequence->getNodes() = std::move(new_self_nodes);
    }

    auto Node::castToAstLangNode(
        const ConvertionTable &conversion_table,
        isl::UniquePtr<ccl::parser::ast::Node>
            node) -> isl::UniquePtr<ccl::parser::ast::Node>
    {
        auto *node_as_sequence = dynamic_cast<ccl::parser::ast::UnNodeSequence *>(node.get());

        if (node_as_sequence == nullptr) {
            return node;
        }

        auto new_node = conversion_table[node_as_sequence->getType()](node_as_sequence);
        dynamic_cast<Node *>(new_node.get())->castChildrenToAstLangNode(conversion_table);

        return new_node;
    }
}// namespace astlang::ast