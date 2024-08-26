#include "ccl/parser/lr/lr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    LrParser::LrParser(
        const LrItem &start_item,
        Symbol epsilon_symbol,
        const GrammarStorage &parser_rules,
        const std::function<std::string(SmallId)> &id_to_string_converter)
    {
        auto parser_generator =
            lr::LrParserGenerator(start_item, epsilon_symbol, parser_rules, id_to_string_converter);

        gotoTable = std::move(parser_generator.getGotoTable());
        actionTable = parser_generator.getLrActionTable();
    }

    auto LrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> UnambiguousParsingResult
    {
        using enum ParsingAction;

        auto parsing_result = UnambiguousParsingResult{.algorithmName = "LR(1)"};
        auto word = ast::SharedNode<ast::TokenNode>{tokenizer.yield()};

        auto parser_state = ParserState{};

        parser_state.stateStack.push(0);

        while (true) {
            const auto state = parser_state.stateStack.top();

            const auto entry = TableEntry{
                .state = state,
                .symbol = word->getType(),
            };

            auto action_table_it = actionTable.find(entry);

            if (action_table_it == actionTable.end()) {
                return parsing_result;
            }

            switch (const auto &action = action_table_it->second; action.getParsingAction()) {
            case SHIFT: {
                auto &[state_stack, nodes_stack] = parser_state;
                nodes_stack.emplace(std::move(word));
                state_stack.emplace(action.getShiftingState());
                word = ast::SharedNode<ast::TokenNode>{tokenizer.yield()};
                break;
            }

            case REDUCE:
                reduceAction(action.getReducingItem(), parser_state);
                break;

            case ACCEPT:
                parsing_result.root = parser_state.nodesStack.top();
                return parsing_result;

            default:
                isl::unreachable();
            }
        }
    }

    auto LrParser::reduceAction(const LrItem &lr_item, ParserState &parser_state) const -> void
    {
        auto &[state_stack, nodes_stack] = parser_state;

        const auto number_of_elements_to_take_from_stack = lr_item.dottedRule.size();

        auto items_in_production = std::vector<ast::SharedNode<>>();
        const auto production = lr_item.getProductionType();

        for (std::size_t i = 0; i != number_of_elements_to_take_from_stack; ++i) {
            items_in_production.emplace_back(std::move(nodes_stack.top()));
            nodes_stack.pop();
            state_stack.pop();
        }

        std::ranges::reverse(items_in_production);

        const auto *rule = lr_item.dottedRule.rule;

        auto reduced_item = rule->construct(production, std::move(items_in_production));

        nodes_stack.emplace(std::move(reduced_item));
        state_stack.emplace(gotoTable.at({
            state_stack.top(),
            production,
        }));
    }
}// namespace ccl::parser