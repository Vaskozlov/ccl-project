#include <ccl/parser/rules_reader/ast/lexer_rule_body.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerRuleBody::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        using namespace lexer::rule;

        auto &lexical_analyzer = parser_builder.getLexicalAnalyzer();
        auto resulted_container = std::make_unique<Container>(lexical_analyzer);

        for (const auto &node : this->nodes) {
            const auto *node_as_rules_reader = static_cast<const RulesReaderNode *>(node.get());
            auto new_block = node_as_rules_reader->construct(parser_builder);

            resulted_container->addItem(isl::get<std::unique_ptr<RuleBlockInterface>>(new_block));
        }

        return isl::UniqueAny{std::move(resulted_container)};
    }
}// namespace ccl::parser::reader::ast