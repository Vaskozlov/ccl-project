#ifndef CCL_PROJECT_LEXER_RULE_BODY_HPP
#define CCL_PROJECT_LEXER_RULE_BODY_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleBody
      : public parser::ast::NodeSequence<isl::UniquePtr>
      , public RulesReaderNode
    {
    public:
        using parser::ast::NodeSequence<isl::UniquePtr>::NodeSequence;

        auto construct(lexer::LexicalAnalyzer &lexical_analyzer) const -> isl::UniqueAny override
        {
            using namespace lexer::rule;

            auto resulted_container =
                isl::makeUnique<Container>(lexical_analyzer, lexical_analyzer.generateIdForItem());

            for (const auto &node : this->nodes) {
                const auto *node_as_rules_reader =
                    dynamic_cast<const RulesReaderNode *>(node.get());
                auto new_block = node_as_rules_reader->construct(lexical_analyzer);

                resulted_container->addItem(isl::anyCast<RuleBlockInterface>(new_block));
            }

            return isl::UniqueAny{std::move(resulted_container)};
        }
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_BODY_HPP */
