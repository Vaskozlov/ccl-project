#ifndef CCL_PROJECT_ASTLANG_NODE_HPP
#define CCL_PROJECT_ASTLANG_NODE_HPP

#include <ast-lang/ast-lang.hpp>
#include <ccl/parser/ast/non_terminal.hpp>
#include <ccl/parser/ast/terminal.hpp>
#include <ccl/parser/rules_reader/parser_builder.hpp>

namespace astlang::interpreter
{
    struct EvaluationResult;
    class Interpreter;
}// namespace astlang::interpreter

namespace astlang::ast
{
    template<std::derived_from<ccl::parser::ast::Node> T = ccl::parser::ast::Node>
    using SharedNode = ccl::parser::ast::SharedNode<T>;

    class Node : public ccl::parser::ast::NonTerminal
    {
    private:
        using ConstructionFunction = ccl::parser::Rule::RuleBuilderFunction;
        using ConversionTable = isl::StaticFlatmap<SmallId, void (*)(NonTerminal *), 50>;

    public:
        using Interpreter = interpreter::Interpreter;
        using EvaluationResult = interpreter::EvaluationResult;
        using NonTerminal::NonTerminal;

        auto runRecursiveOptimization() -> void;

        virtual auto optimize() -> SharedNode<>;

        virtual auto compute(Interpreter &interpreter) const -> EvaluationResult = 0;

        static auto convertCclTreeToAstlang(
            ccl::parser::reader::ParserBuilder &constructor,
            Node *node) -> void;

        auto castChildrenToAstLangNode(const ConversionTable &conversion_table) -> void;

        static auto
            castToAstLangNode(const ConversionTable &conversion_table, SharedNode<> node) -> void;
    };

    struct NodePtr
    {
        union {
            ccl::parser::ast::Node *cclNode;
            ccl::parser::ast::Terminal *terminalNode;
            ccl::parser::ast::NonTerminal *nonTerminalNode;
            Node *astlangNode;
        };

        explicit NodePtr(ccl::parser::ast::Node *node)
          : cclNode{node}
        {}
    };

    struct ConstNodePtr
    {
        union {
            const ccl::parser::ast::Node *cclNode;
            const ccl::parser::ast::Terminal *terminalNode;
            const ccl::parser::ast::NonTerminal *nonTerminalNode;
            const Node *astlangNode;
        };

        explicit ConstNodePtr(const ccl::parser::ast::Node *node)
          : cclNode{node}
        {}
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_NODE_HPP */
