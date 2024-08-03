#include "ccl/parser/rules_reader/ast/lexer_rule_options.hpp"
#include <isl/flatmap.hpp>

namespace ccl::parser::reader::ast
{
    constexpr static isl::StaticFlatmap<RulesLexerToken, char, 5> OptionToRep{
        {
            RulesLexerToken::HIDE_OPERATOR,
            'h',
        },
        {
            RulesLexerToken::NOT_OPERATOR,
            '^',
        },
        {
            RulesLexerToken::PREFIX_POSTFIX_OPERATOR,
            'p',
        },
        {
            RulesLexerToken::PLUS,
            '+',
        },
        {
            RulesLexerToken::STAR,
            '*',
        },
    };

    LexerRuleOptions::LexerRuleOptions(
        SmallId node_type_id, const std::initializer_list<RulesLexerToken> &rule_options)
      : parser::ast::StringNode{node_type_id, ""}
    {
        for (const auto option : rule_options) {
            addOption(option);
        }
    }

    auto LexerRuleOptions::addOption(RulesLexerToken option) -> void
    {
        repr.push_back(OptionToRep.at(option));
        options.emplace_back(option);
    }

    auto LexerRuleOptions::construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny
    {
        return std::nullopt;
    }
}// namespace ccl::parser::reader::ast