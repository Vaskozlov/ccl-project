#ifndef CCL_PROJECT_ASTLANG_NODE_HPP
#define CCL_PROJECT_ASTLANG_NODE_HPP

#include <ast-lang/ast-lang.hpp>
#include <ccl/parser/ast/node_of_nodes.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/parser_builder.hpp>

namespace astlang::interpreter
{
    struct EvaluationResult;
    class Interpreter;
}// namespace astlang::interpreter

namespace astlang::ast
{
    class Node : public ccl::parser::ast::NodeOfNodes
    {
    private:
        using ConstructionFunction = ccl::parser::Rule::RuleBuilderFunction;
        using ConversionTable = isl::
            StaticFlatmap<SmallId, std::function<ccl::parser::ast::Node *(NodeOfNodes *node)>, 50>;

        template<typename T>
        static auto reconstructNode(NodeOfNodes *node) -> NodeOfNodes *
        {
            return node->emplaceAfter<T>(node->getType(), std::move(node->getNodes()));
        }

    public:
        using Interpreter = interpreter::Interpreter;
        using EvaluationResult = interpreter::EvaluationResult;
        using NodeOfNodes::NodeOfNodes;

        auto runRecursiveOptimization() -> void;

        virtual auto optimize() -> ccl::parser::ast::Node *;

        virtual auto compute(Interpreter &interpreter) const -> EvaluationResult = 0;

        static auto convertCclTreeToAstlang(
            ccl::parser::reader::ParserBuilder &constructor,
            Node *node) -> void;

        auto castChildrenToAstLangNode(const ConversionTable &conversion_table) -> void;

        static auto castToAstLangNode(
            const ConversionTable &conversion_table,
            ccl::parser::ast::Node *node) -> ccl::parser::ast::Node *;
    };

    struct NodePtr
    {
        union {
            ccl::parser::ast::Node *cclNode;
            ccl::parser::ast::TokenNode *tokenNode;
            ccl::parser::ast::NodeOfNodes *nodeSequence;
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
            const ccl::parser::ast::TokenNode *tokenNode;
            const ccl::parser::ast::NodeOfNodes *nodeSequence;
            const Node *astlangNode;
        };

        explicit ConstNodePtr(const ccl::parser::ast::Node *node)
          : cclNode{node}
        {}
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_NODE_HPP */
