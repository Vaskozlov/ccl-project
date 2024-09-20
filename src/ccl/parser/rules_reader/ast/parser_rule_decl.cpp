#include <ccl/parser/ast/terminal.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule_decl.hpp>

namespace ccl::parser::reader::ast
{
    auto ParserRuleDecl::getDeclarationName() const -> isl::string_view
    {
        const auto *value_node = front().get();
        const auto *declaration_token = dynamic_cast<const parser::ast::Terminal *>(value_node);
        const auto &token = declaration_token->getToken();

        return token.getExtractedParts().front();
    }

    auto ParserRuleDecl::getAlternatives() const -> const ParserRuleAlternatives *
    {
        const auto *value_node = back().get();
        return dynamic_cast<const ParserRuleAlternatives *>(value_node);
    }

    auto ParserRuleDecl::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto production_name = getDeclarationName();
        const auto production_id =
            parser_builder.addRule(static_cast<std::string>(production_name));

        const auto *alternatives = getAlternatives();

        if (alternatives == nullptr) {
            auto not_casted_rule =
                static_cast<const RulesReaderNode *>(back().get())->construct(parser_builder);

            parser_builder.addParserRule(
                production_id, isl::get<std::vector<SmallId>>(not_casted_rule));

            return std::nullopt;
        }

        auto not_casted_rules = alternatives->construct(parser_builder);
        const auto rules = not_casted_rules.release<std::vector<std::vector<SmallId>>>();

        for (auto &alternative : *rules) {
            parser_builder.addParserRule(production_id, std::move(alternative));
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast
