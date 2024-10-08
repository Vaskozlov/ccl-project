#include "ccl/parser/lr/lr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/terminal.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    LrParser::LrParser(
        const LrItem &start_item,
        const Symbol epsilon_symbol,
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
        auto token = ast::SharedNode<ast::Terminal>{tokenizer.yield()};

        auto parser_state = ParserState{};

        parser_state.stateStack.emplace();

        while (true) {
            const auto state = parser_state.stateStack.top();

            auto action_table_it = actionTable.find({
                .state = state,
                .symbol = token->getType(),
            });

            if (action_table_it == actionTable.end()) {
                return parsing_result;
            }

            switch (const Action &action = action_table_it->second; action.getActionType()) {
            case SHIFT:
                parser_state.nodesStack.emplace(std::move(token));
                parser_state.stateStack.emplace(action.getShiftingState());
                token = ast::SharedNode<ast::Terminal>{tokenizer.yield()};
                break;

            case REDUCE:
                reduceAction(
                    action.getProductionType(),
                    action.getNumberOfElementsInProduction(),
                    parser_state);
                break;

            case ACCEPT:
                parsing_result.root = parser_state.nodesStack.top();
                return parsing_result;

            default:
                isl::unreachable();
            }
        }
    }

    auto LrParser::reduceAction(
        Symbol production_type,
        const SmallId number_of_elements,
        ParserState &parser_state) const -> void
    {
        auto &[state_stack, nodes_stack] = parser_state;
        auto reduced_item = ast::SharedNode<ast::NonTerminal>{production_type};

        for (auto i = std::size_t{}; i != number_of_elements; ++i) {
            reduced_item->addNode(std::move(nodes_stack.top()));
            nodes_stack.pop();
            state_stack.pop();
        }

        reduced_item->reverse();
        nodes_stack.emplace(std::move(reduced_item));

        state_stack.emplace(gotoTable.at({
            .state = state_stack.top(),
            .symbol = production_type,
        }));
    }
}// namespace ccl::parser