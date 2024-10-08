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
            EXTRACTABLE_MODIFIER,
            'e',
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
        const auto *front_node = front().get();
        auto options = isl::SmallVector<SmallId, 4>{front_node->getType()};

        if (size() > 1) {
            const auto *other_options_node = static_cast<const LexerRuleOptions *>(back().get());

            auto other_options = other_options_node->construct(parser_builder)
                                     .release<isl::SmallVector<SmallId, 4>>();

            for (auto option : *other_options) {
                options.emplace_back(option);
            }
        }

        return isl::UniqueAny{std::move(options)};
    }
}// namespace ccl::parser::reader::ast
