#include <ccl/parser/rules_reader/ast/lexer_rule_decl.hpp>

namespace ccl::parser::reader::ast
{
    auto LexerRuleDecl::getDeclarationName() const -> isl::string_view
    {
        const auto *value_node = this->front();
        const auto *declaration_token = dynamic_cast<const parser::ast::TokenNode *>(value_node);
        const auto &token = declaration_token->getToken();
        return token.getPrefixes().front();
    }

    auto LexerRuleDecl::getRules() const -> const ast::LexerRuleBlock *
    {
        const auto *value_node = this->back();
        return dynamic_cast<const ast::LexerRuleBlock *>(value_node);
    }

    auto LexerRuleDecl::isSpecial() const noexcept -> bool
    {
        constexpr auto number_of_blocks_when_special = 3;
        return this->size() == number_of_blocks_when_special;
    }

    auto LexerRuleDecl::construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny
    {
        auto rule_name = getDeclarationName();

        const auto *last_node = this->back();
        const auto *rule_node = dynamic_cast<const RulesReaderNode *>(last_node);

        auto rule = rule_node->construct(rule_constructor);
        auto rule_as_container = isl::anyCast<lexer::rule::Container>(rule);

        if (isSpecial()) {
            rule_as_container->makeAsAnyPlaceItem();
        }

        rule_constructor.addLexerRule(rule_name, std::move(rule_as_container));
        return isl::makeAny<isl::string_view>(rule_name);
    }
}// namespace ccl::parser::reader::ast