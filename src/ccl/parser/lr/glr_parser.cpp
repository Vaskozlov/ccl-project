#include "ccl/parser/lr/glr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/dot/dot_repr.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"
#include "ccl/parser/lr/gss/gss.hpp"
#include <isl/dot_repr.hpp>
#include <isl/io.hpp>

namespace ccl::parser
{
    using enum ParsingAction;

    static auto debugGlr(lr::GSS &gss, auto &&function) -> void
    {
        auto result = std::vector<ast::Node *>{};

        for (const auto &level : gss.getLevels()) {
            result.emplace_back(level.terminal->value.get());

            for (const auto &node : level.nonTerminals) {
                result.emplace_back(node->value.get());
            }
        }

        auto tree_repr = dot::createDotRepresentation(result, function);
        isl::io::writeToFile(std::filesystem::current_path().append("glr.dot"), tree_repr);
    }

    GlrParser::GlrParser(
        const LrItem &start_item,
        Symbol epsilon_symbol,
        const GrammarStorage &parser_rules,
        const std::function<std::string(SmallId)> &id_to_string_converter)
      : idToStringConverter{id_to_string_converter}
    {
        auto parser_generator =
            lr::LrParserGenerator(start_item, epsilon_symbol, parser_rules, idToStringConverter);

        gotoTable = std::move(parser_generator.getGotoTable());
        actionTable = parser_generator.getGlrActionTable();
    }

    auto GlrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> AmbiguousParsingResult
    {
        using enum ParsingAction;

        auto parsing_result = AmbiguousParsingResult{.algorithmName = "GLR"};
        auto token = ast::SharedNode<ast::TokenNode>(tokenizer.yield());

        auto gss = lr::GSS{};
        gss.nextWord();

        auto *start_node = gss.pushTerminal(nullptr, 0, 0, token);

        gss.add({
            .stack = start_node,
            .inputPosition = 0,
            .parserState = 0,
        });

        while (gss.hasDescriptors()) {
            auto descriptor = gss.getDescriptor();
            const auto parser_state = descriptor.parserState;

            if (descriptor.inputPosition == gss.getGlobalInputPosition()) {
                token = ast::SharedNode<ast::TokenNode>(tokenizer.yield());
                gss.nextWord();
            }

            const auto entry = TableEntry{
                .state = parser_state,
                .symbol = token->getType(),
            };

            auto possible_actions_it = actionTable.find(entry);

            if (possible_actions_it == actionTable.end()) {
                continue;
            }

            const auto &possible_actions = possible_actions_it->second;

            for (const auto &action : possible_actions) {
                switch (action.getParsingAction()) {
                case SHIFT: {
                    auto *new_node = gss.pushTerminal(
                        descriptor.stack,
                        descriptor.inputPosition,
                        action.getShiftingState(),
                        token);

                    gss.add({
                        .stack = new_node,
                        .inputPosition = descriptor.inputPosition + 1,
                        .parserState = action.getShiftingState(),
                    });
                    break;
                }

                case REDUCE: {
                    const auto &lr_item = action.getReducingItem();
                    const auto production = lr_item.production;
                    const auto *rule = lr_item.dottedRule.rule;
                    const auto number_of_elements_to_pop =
                        static_cast<SmallId>(lr_item.dottedRule.size());

                    auto reducer = [rule, production](std::vector<ast::SharedNode<>> nodes) {
                        std::ranges::reverse(nodes);
                        return rule->construct(production, std::move(nodes));
                    };

                    gss.reduce(
                        number_of_elements_to_pop,
                        &gotoTable,
                        production,
                        std::move(reducer),
                        descriptor);

                    break;
                }

                case ACCEPT:
                    parsing_result.roots.emplace_back(descriptor.stack->value);
                    break;

                default:
                    isl::unreachable();
                }
            }

            // debugGlr(gss, idToStringConverter);
        }

        return parsing_result;
    }
}// namespace ccl::parser
