#ifndef CCL_PROJECT_ASTLANG_NODE_HPP
#define CCL_PROJECT_ASTLANG_NODE_HPP

#include <ast-lang/ast-lang.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/rules_constructor.hpp>

namespace astlang::interpreter
{
    struct EvaluationResult;
    class Interpreter;
}// namespace astlang::interpreter

namespace astlang::ast
{
    class Node : public ccl::parser::ast::UnNodeSequence
    {
    private:
        using ConstructionFunction =
            isl::UniquePtr<ccl::parser::ast::Node> (*)(ccl::parser::ast::UnNodeSequence *);

        using ConvertionTable = isl::StaticFlatmap<SmallId, ConstructionFunction, 35>;

        template<typename T>
        static auto reconstructNode(ccl::parser::ast::UnNodeSequence *node)
            -> isl::UniquePtr<ccl::parser::ast::Node>
        {
            return isl::makeUnique<T>(node->getType(), std::move(node->getNodes()));
        }

    public:
        using Interpreter = interpreter::Interpreter;
        using EvaluationResult = interpreter::EvaluationResult;
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        virtual auto compute(Interpreter &interpreter) -> EvaluationResult = 0;

        static auto convertCclTreeToAstlang(
            ccl::parser::reader::RulesConstructor &constructor,
            Node *node) -> void;

        auto castChildrenToAstLangNode(const ConvertionTable &conversion_table) -> void;

        static auto castToAstLangNode(
            const ConvertionTable &conversion_table,
            isl::UniquePtr<ccl::parser::ast::Node>
                node) -> isl::UniquePtr<ccl::parser::ast::Node>;
    };

    struct NodePtr
    {
        union {
            ccl::parser::ast::Node *cclNode;
            ccl::parser::ast::TokenNode *tokenNode;
            ccl::parser::ast::UnNodeSequence *nodeSequence;
            Node *astlangNode;
        };

        explicit NodePtr(ccl::parser::ast::Node *node)
          : cclNode{node}
        {}
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_NODE_HPP */
