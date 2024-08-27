#include <ccl/parser/rules_reader/ast/lexer_block.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerBlock::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto *next_block = dynamic_cast<const LexerBlock *>(back().get());

        const auto *rule_identifier = static_cast<const parser::ast::TokenNode *>(front().get());
        const auto *exclamation_point = dynamic_cast<parser::ast::TokenNode *>(at(1).get());

        const auto *rule_alternative = static_cast<const RulesReaderNode *>(
            exclamation_point == nullptr ? at(1).get() : at(2).get());

        const lexer::Token &rule_identifier_token = rule_identifier->getToken();
        const auto rule_name =
            static_cast<std::string>(rule_identifier_token.getPrefixes().front());

        auto rule_block = isl::get<std::unique_ptr<lexer::rule::Container>>(
            rule_alternative->construct(parser_builder));

        parser_builder.addLexerRule(rule_name, std::move(rule_block));

        if (next_block != nullptr) {
            std::ignore = next_block->construct(parser_builder);
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast
