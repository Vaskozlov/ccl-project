#include "ccl/parser/rules_reader/ast/lexer_rule_options.hpp"

#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <isl/static_flat_map.hpp>

namespace ccl::parser::reader::ast
{
    constexpr static isl::StaticFlatmap<RulesLexerToken, char, 5> OptionToRep{
        {
            HIDE_OPERATOR,
            'h',
        },
        {
            NOT_OPERATOR,
            '^',
        },
        {
            PREFIX_POSTFIX_OPERATOR,
            'p',
        },
        {
            PLUS,
            '+',
        },
        {
            STAR,
            '*',
        },
    };

    auto LexerRuleOptions::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        const auto *front_node = this->front().get();
        auto options = std::vector{static_cast<RulesLexerToken>(front_node->getType())};

        if (this->size() > 1) {
            const auto *other_options_node = this->back().get();
            const auto *casted_other_options =
                static_cast<const LexerRuleOptions *>(other_options_node);

            auto other_options = isl::get<std::vector<RulesLexerToken>>(
                casted_other_options->construct(parser_builder));

            options.insert(options.end(), other_options.begin(), other_options.end());
        }

        return isl::UniqueAny{std::move(options)};
    }
}// namespace ccl::parser::reader::ast
