#include <ccl/parser/rules_reader/ast/parser_rule_decl.hpp>

namespace ccl::parser::reader::ast
{
    auto ParserRuleDecl::getDeclarationName() const -> isl::string_view
    {
        const auto *value_node = this->front();
        const auto *declaration_token = dynamic_cast<const parser::ast::TokenNode *>(value_node);
        const auto &token = declaration_token->getToken();

        return token.getPrefixes().front();
    }

    auto ParserRuleDecl::getAlternatives() const -> const ast::ParserRuleAlternatives *
    {
        const auto *value_node = this->back();
        return dynamic_cast<const ast::ParserRuleAlternatives *>(value_node);
    }

    auto ParserRuleDecl::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto production_name = getDeclarationName();
        const auto production_id = parser_builder.addRule(production_name);
        const auto *alternatives = getAlternatives();

        if (alternatives == nullptr) {
            auto not_casted_rule =
                static_cast<const RulesReaderNode *>(back())->construct(parser_builder);
            auto rule = isl::get<std::vector<SmallId>>(not_casted_rule);

            parser_builder.addParserRule(production_id, parser::Rule{{rule}});
            return std::nullopt;
        }

        auto not_casted_rules = alternatives->construct(parser_builder);
        const auto rules = isl::get<std::vector<std::vector<SmallId>>>(not_casted_rules);

        for (const auto &alternative : rules) {
            parser_builder.addParserRule(production_id, parser::Rule{alternative});
        }

        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast
