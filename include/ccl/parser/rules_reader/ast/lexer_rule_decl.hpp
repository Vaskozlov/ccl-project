#ifndef CCL_PROJECT_LEXER_RULE_DECL_HPP
#define CCL_PROJECT_LEXER_RULE_DECL_HPP

#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleDecl
      : public parser::ast::NodeSequence<isl::UniquePtr>
      , public RulesReaderNode
    {
    public:
        using parser::ast::NodeSequence<isl::UniquePtr>::NodeSequence;

        [[nodiscard]] auto getDeclarationName() const -> const parser::ast::TokenNode *
        {
            const auto *value_node = this->front();
            return isl::as<const parser::ast::TokenNode *>(value_node);
        }

        [[nodiscard]] auto getRules() const -> const ast::LexerRuleBlock *
        {
            const auto *value_node = this->back();
            return isl::as<const ast::LexerRuleBlock *>(value_node);
        }

        [[nodiscard]] auto isSpecial() const noexcept -> bool
        {
            constexpr auto number_of_blocks_when_special = 3;
            return this->size() == number_of_blocks_when_special;
        }

        auto construct(lexer::LexicalAnalyzer &lexical_analyzer) const -> isl::UniqueAny override
        {
            const auto *first_node = this->front();
            const auto *last_node = this->back();

            const auto *rule_name_node = isl::as<const parser::ast::TokenNode *>(first_node);
            auto rule_name = rule_name_node->getToken().getPrefixes().front();

            const auto *rule_node = dynamic_cast<const RulesReaderNode *>(last_node);
            auto rule = rule_node->construct(lexical_analyzer);
            auto rule_as_container = isl::anyCast<lexer::rule::Container>(rule);

            rule_as_container->makeAsAnyPlaceItem();
            lexical_analyzer.addContainer(rule_name, std::move(rule_as_container));

            return isl::makeAny<isl::string_view>(rule_name);
        }
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_DECL_HPP */
