#include "ccl/parser/lr/lr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/node_sequence.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr/lr_parser_generator.hpp"

namespace ccl::parser
{
    LrParser::LrParser(
        const LrItem &start_item, Symbol epsilon_symbol, const isl::Set<Symbol> &grammar_symbols,
        const isl::Set<Symbol> &terminal_symbols, const GrammarRulesStorage &parser_rules)
    {
        auto parser_generator = LrParserGenerator(
            start_item, epsilon_symbol, grammar_symbols, terminal_symbols, parser_rules);

        gotoTable = std::move(parser_generator.getGotoTable());
        actionTable = parser_generator.getLrActionTable();
    }

    auto LrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> std::unique_ptr<ast::Node>
    {
        using enum ccl::parser::ParsingAction;

        auto state_stack = isl::Stack<State>{};
        auto nodes_stack = isl::Stack<ast::UnNodePtr>{};
        const auto *word = &tokenizer.yield();

        state_stack.push(0);

        while (true) {
            const auto state = state_stack.top();
            const auto entry = TableEntry{
                .state = state,
                .lookAhead = word->getId(),
            };

            if (!actionTable.contains(entry)) {
                return nullptr;
            }

            const auto &action = actionTable.at(entry);

            switch (action.getParsingAction()) {
            case SHIFT:
                nodes_stack.emplace(isl::makeUnique<ast::TokenNode>(word->getId(), *word));
                state_stack.emplace(action.getShiftingState());
                word = &tokenizer.yield();
                break;

            case REDUCE:
                reduceAction(action, state_stack, nodes_stack);
                break;

            case ACCEPT:
                return std::move(nodes_stack.top());

            default:
                isl::unreachable();
            }
        }
    }

    auto LrParser::reduceAction(
        const Action &action,
        isl::Stack<State> &state_stack,
        isl::Stack<ast::UnNodePtr> &nodes_stack) const -> void
    {
        const auto &lr_item = action.getReducingItem();
        auto reduced_item =
            isl::makeUnique<ast::UnNodeSequence>(isl::as<Symbol>(lr_item.getProductionType()));
        const auto number_of_elements_to_take_from_stack = lr_item.length();

        for (std::size_t i = 0; i != number_of_elements_to_take_from_stack; ++i) {
            reduced_item->addNode(std::move(nodes_stack.top()));
            nodes_stack.pop();
            state_stack.pop();
        }

        reduced_item->reverse();

        nodes_stack.emplace(std::move(reduced_item));
        state_stack.emplace(gotoTable.at({
            state_stack.top(),
            lr_item.getProductionType(),
        }));
    }
}// namespace ccl::parser