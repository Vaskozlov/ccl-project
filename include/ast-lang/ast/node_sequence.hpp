#ifndef CCL_PROJECT_ASTLANG_NODE_SEQUENCE_HPP
#define CCL_PROJECT_ASTLANG_NODE_SEQUENCE_HPP

#include <ast-lang/ast/node.hpp>
#include <ast-lang/interpreter/evaluation_result.hpp>
#include <ccl/parser/rules_reader/rules_constructor.hpp>

namespace astlang::ast
{
    class NodeSequence : public Node
    {
        template<typename T>
        static auto reconstructNode(ccl::parser::ast::UnNodeSequence *node)
            -> isl::UniquePtr<ccl::parser::ast::Node>
        {
            return isl::makeUnique<T>(node->getType(), std::move(node->getNodes()));
        }

    public:
        using Node::Node;
        using ConvertionTable = isl::StaticFlatmap<
            SmallId,
            isl::UniquePtr<ccl::parser::ast::Node> (*)(ccl::parser::ast::UnNodeSequence *),
            35>;

        static auto convertCclTreeToAstlang(
            ccl::parser::reader::RulesConstructor &constructor,
            NodeSequence *node) -> void;

        auto castChildrenToAstLangNode(const ConvertionTable &conversion_table) -> void;

        static auto castToAstLangNode(
            const ConvertionTable &conversion_table,
            isl::UniquePtr<ccl::parser::ast::Node>
                node) -> isl::UniquePtr<ccl::parser::ast::Node>;
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_NODE_SEQUENCE_HPP */
