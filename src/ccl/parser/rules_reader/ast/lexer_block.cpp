#include <ccl/parser/rules_reader/ast/lexer_block.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerBlock::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto *next_block = dynamic_cast<const LexerBlock *>(back().get());

        const auto *rule_identifier = static_cast<const parser::ast::Terminal *>(front().get());
        const auto *exclamation_point = dynamic_cast<parser::ast::Terminal *>(at(1).get());
        const auto is_any_place_item = exclamation_point != nullptr;

        const auto *rule_alternative =
            static_cast<const RulesReaderNode *>(at(1 + static_cast<u32>(is_any_place_item)).get());

        const lexer::Token &rule_identifier_token = rule_identifier->getToken();
        const auto rule_name =
            static_cast<std::string>(rule_identifier_token.getExtractedParts().front());

        auto rule_block =
            rule_alternative->construct(parser_builder).release<lexer::rule::Container>();

        if (is_any_place_item) {
            rule_block->makeAsAnyPlaceItem();
        }

        parser_builder.addLexerRule(rule_name, std::move(rule_block));

        if (next_block != nullptr) {
            std::ignore = next_block->construct(parser_builder);
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast
