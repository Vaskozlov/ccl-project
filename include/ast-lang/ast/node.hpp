#ifndef CCL_PROJECT_ASTLANG_NODE_HPP
#define CCL_PROJECT_ASTLANG_NODE_HPP

#include <ast-lang/ast-lang.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>

namespace astlang::interpreter
{
    struct EvaluationResult;
    class Interpreter;
}// namespace astlang::interpreter

namespace astlang::ast
{
    class Node
    {
    public:
        Node() = default;

        Node(const Node &) = delete;
        Node(Node &&) noexcept = default;

        virtual ~Node() = default;

        auto operator=(const Node &) -> void = delete;
        auto operator=(Node &&) -> void = delete;

        virtual auto
            compute(interpreter::Interpreter &interpreter) -> interpreter::EvaluationResult = 0;
    };

    struct NodePtr
    {
        union {
            ccl::parser::ast::Node *cclNode;
            ccl::parser::ast::TokenNode *tokenNode;
            ccl::parser::ast::UnNodeSequence *nodeSequence;
        };

        Node *astlangNode;

        explicit NodePtr(ccl::parser::ast::Node *node)
          : cclNode{node}
          , astlangNode{dynamic_cast<Node *>(node)}
        {}
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_NODE_HPP */
