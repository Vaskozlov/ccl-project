#include <ccl/parser/rules_reader/ast/lexer_rule_body.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerRuleBody::construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny
    {
        using namespace lexer::rule;

        auto &lexical_analyzer = rule_constructor.getLexicalAnalyzer();
        auto resulted_container =
            isl::makeUnique<Container>(lexical_analyzer, lexical_analyzer.generateIdForItem());

        for (const auto &node : this->nodes) {
            const auto *node_as_rules_reader = dynamic_cast<const RulesReaderNode *>(node.get());
            auto new_block = node_as_rules_reader->construct(rule_constructor);

            resulted_container->addItem(isl::anyCast<RuleBlockInterface>(new_block));
        }

        return isl::UniqueAny{std::move(resulted_container)};
    }
}// namespace ccl::parser::reader::ast