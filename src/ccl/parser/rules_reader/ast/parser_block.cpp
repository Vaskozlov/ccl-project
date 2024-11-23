#include <ccl/parser/ast/terminal.hpp>
#include <ccl/parser/rules_reader/ast/parser_block.hpp>

namespace ccl::parser::reader::ast
{
    auto ParserBlock::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto *rule_identifier = static_cast<const parser::ast::Terminal *>(front().get());
        const auto *rule_alternative = static_cast<const RulesReaderNode *>(at(1).get());

        const lexer::Token &rule_identifier_token = rule_identifier->getToken();

        const auto rule_name = rule_identifier_token.getExtractedParts().front();

        auto parser_alternatives =
            isl::get<std::vector<std::vector<Symbol>>>(rule_alternative->construct(parser_builder));

        for (auto &alternative : parser_alternatives) {
            parser_builder.addParserRule(
                static_cast<std::string>(rule_name), std::move(alternative));
        }

        if (size() == 3) {
            const auto *next_block = static_cast<RulesReaderNode *>(back().get());
            std::ignore = next_block->construct(parser_builder);
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast
