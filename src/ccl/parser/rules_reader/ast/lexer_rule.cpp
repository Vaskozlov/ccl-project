#include <ccl/parser/rules_reader/ast/lexer_rule.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerRule::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        using namespace lexer::rule;

        auto &lexical_analyzer = parser_builder.getLexicalAnalyzer();

        if (size() == 1) {
            auto container = std::make_unique<Container>(lexical_analyzer);
            const auto *first_block = static_cast<const RulesReaderNode *>(front().get());
            container->addItem(isl::get<RuleBlock>(first_block->construct(parser_builder)));

            return isl::UniqueAny{std::move(container)};
        }

        const auto *constructed_rule_blocks = static_cast<const RulesReaderNode *>(front().get());
        auto container = isl::get<std::unique_ptr<Container>>(
            constructed_rule_blocks->construct(parser_builder));

        const auto *next_block = static_cast<const RulesReaderNode *>(back().get());
        container->addItem(isl::get<RuleBlock>(next_block->construct(parser_builder)));

        return isl::UniqueAny{std::move(container)};
    }
}// namespace ccl::parser::reader::ast