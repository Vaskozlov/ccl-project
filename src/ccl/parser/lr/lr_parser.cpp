#include "ccl/parser/lr/lr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    LrParser::LrParser(
        const GrammarSlot &start_item,
        Symbol epsilon_symbol,
        const GrammarStorage &parser_rules,
        std::function<std::string(SmallId)>
            id_to_string_converter)
    {
        auto parser_generator =
            LrParserGenerator(start_item, epsilon_symbol, parser_rules, id_to_string_converter);

        gotoTable = std::move(parser_generator.getGotoTable());
        actionTable = parser_generator.getLrActionTable();
    }

    auto LrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> std::pair<ast::Node *, isl::DynamicForwardList<ast::Node>>
    {
        using enum ccl::parser::ParsingAction;

        auto state_stack = Stack<State>{};
        auto nodes_stack = Stack<ast::Node *>{};
        auto forward_list = isl::DynamicForwardList<ast::Node>{};
        const auto *word = &tokenizer.yield();

        state_stack.push(0);

        while (true) {
            const auto state = state_stack.top();
            const auto entry = TableEntry{
                .state = state,
                .symbol = word->getId(),
            };

            if (!actionTable.contains(entry)) {
                return {nullptr, std::move(forward_list)};
            }

            const auto &action = actionTable.at(entry);

            switch (action.getParsingAction()) {
            case SHIFT:
                nodes_stack.emplace(
                    forward_list.emplaceFront<ast::TokenNode>(word->getId(), *word));
                state_stack.emplace(action.getShiftingState());
                word = &tokenizer.yield();
                break;

            case REDUCE:
                reduceAction(action, state_stack, nodes_stack);
                break;

            case ACCEPT:
                return {nodes_stack.top(), std::move(forward_list)};

            default:
                isl::unreachable();
            }
        }
    }

    auto LrParser::reduceAction(
        const Action &action,
        Stack<State> &state_stack,
        Stack<ast::Node *> &nodes_stack) const -> void
    {
        const auto &lr_item = action.getReducingItem();
        const auto production = lr_item.getProductionType();
        auto items_in_production = std::vector<ast::Node *>();
        const auto number_of_elements_to_take_from_stack = lr_item.size();

        for (std::size_t i = 0; i != number_of_elements_to_take_from_stack; ++i) {
            items_in_production.emplace_back(nodes_stack.top());
            nodes_stack.pop();
            state_stack.pop();
        }

        std::ranges::reverse(items_in_production);

        const auto *rule = lr_item.getRulePtr();
        auto reduced_item = rule->construct(production, std::move(items_in_production));

        nodes_stack.emplace(std::move(reduced_item));
        state_stack.emplace(gotoTable.at({
            state_stack.top(),
            production,
        }));
    }
}// namespace ccl::parser