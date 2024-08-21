#include "ccl/parser/lr/lr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    LrParser::LrParser(
        const LrItem&start_item,
        Symbol epsilon_symbol,
        const GrammarStorage &parser_rules,
        const std::function<std::string(SmallId)> &id_to_string_converter)
    {
        auto parser_generator =
            LrParserGenerator(start_item, epsilon_symbol, parser_rules, id_to_string_converter);

        gotoTable = std::move(parser_generator.getGotoTable());
        actionTable = parser_generator.getLrActionTable();
    }

    auto LrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> UnambiguousParsingResult
    {
        using enum ParsingAction;

        const auto *word = &tokenizer.yield();

        auto parsing_result = UnambiguousParsingResult{};
        auto parser_state = ParserState{
            .nodesLifetimeManager = parsing_result.nodesLifetimeManager.get(),
            .stateStack = {},
            .nodesStack = {},
        };

        parser_state.stateStack.push(0);

        while (true) {
            const auto state = parser_state.stateStack.top();

            const auto entry = TableEntry{
                .state = state,
                .symbol = word->getId(),
            };

            if (!actionTable.contains(entry)) {
                return parsing_result;
            }

            switch (const auto &action = actionTable.at(entry); action.getParsingAction()) {
            case SHIFT:
                shiftAction(word, action.getShiftingState(), parser_state);
                word = std::addressof(tokenizer.yield());
                break;

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

    auto LrParser::shiftAction(
        const lexer::Token *word,
        State shifting_state,
        ParserState &parser_state) const -> void
    {
        auto &[nodes_lifetime_manager, state_stack, nodes_stack] = parser_state;

        nodes_stack.emplace(nodes_lifetime_manager->create<ast::TokenNode>(word->getId(), *word));
        state_stack.emplace(shifting_state);
    }

    auto LrParser::reduceAction(const LrItem &lr_item, ParserState &parser_state) const -> void
    {
        auto &[nodes_lifetime_manager, state_stack, nodes_stack] = parser_state;

        const auto number_of_elements_to_take_from_stack = lr_item.dottedRule.size();

        auto items_in_production = std::vector<ast::Node *>();
        const auto production = lr_item.getProductionType();

        for (std::size_t i = 0; i != number_of_elements_to_take_from_stack; ++i) {
            items_in_production.emplace_back(nodes_stack.top());
            nodes_stack.pop();
            state_stack.pop();
        }

        std::ranges::reverse(items_in_production);

        const auto *rule = lr_item.dottedRule.rule;

        auto *reduced_item = rule->construct(production, std::move(items_in_production));
        nodes_lifetime_manager->insert(reduced_item);

        nodes_stack.emplace(reduced_item);
        state_stack.emplace(gotoTable.at({
            state_stack.top(),
            production,
        }));
    }
}// namespace ccl::parser