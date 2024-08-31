#include <ccl/parser/rules_reader/ast/lexer_rule.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerRule::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        using namespace lexer::rule;

        auto &lexical_analyzer = parser_builder.getLexicalAnalyzer();

        if (size() == 1) {
            auto result = isl::makeAny<Container>(lexical_analyzer);
            const auto *first_block = static_cast<const RulesReaderNode *>(front().get());

            isl::observe<Container>(result)->addItem(
                RuleBlock{first_block->construct(parser_builder).release<RuleBlockInterface>()});

            return result;
        }

        const auto *constructed_rule_blocks = static_cast<const RulesReaderNode *>(front().get());
        auto container = constructed_rule_blocks->construct(parser_builder);

        const auto *next_block = static_cast<const RulesReaderNode *>(back().get());

        isl::observe<Container>(container)->addItem(
            RuleBlock{next_block->construct(parser_builder).release<RuleBlockInterface>()});

        return container;
    }
}// namespace ccl::parser::reader::ast