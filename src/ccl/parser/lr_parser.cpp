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
        actionTable = std::move(parser_generator.getActionTable());
    }

    auto LrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> std::unique_ptr<ast::Node>
    {
        using enum ccl::parser::ParsingAction;

        auto state_stack = isl::Vector<State>{0};
        auto nodes_stack = isl::Vector<ast::NodePtr>{};
        const auto *word = &tokenizer.yield();

        while (true) {
            const auto state = state_stack.back();
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
                nodes_stack.emplace_back(isl::makeUnique<ast::TokenNode>(word->getId(), *word));
                state_stack.push_back(action.getShiftingState());
                word = &tokenizer.yield();
                break;

            case REDUCE:
                reduceAction(action, state_stack, nodes_stack);
                break;

            case ACCEPT:
                return std::move(nodes_stack.back());

            default:
                isl::unreachable();
            }
        }
    }

    auto LrParser::reduceAction(
        const Action &action,
        isl::Vector<State> &state_stack,
        isl::Vector<ast::NodePtr> &nodes_stack) const -> void
    {
        const auto &lr_item = action.getReducingItem();
        auto reduced_item =
            isl::makeUnique<ast::NodeSequence>(isl::as<Symbol>(lr_item.getProductionType()));
        const auto number_of_elements_to_take_from_stack = lr_item.length();

        for (std::size_t i = 0; i != number_of_elements_to_take_from_stack; ++i) {
            reduced_item->addNode(std::move(nodes_stack.back()));
            nodes_stack.pop_back();
            state_stack.pop_back();
        }

        reduced_item->reverse();
        nodes_stack.push_back(std::move(reduced_item));
        state_stack.push_back(gotoTable.at({
            state_stack.back(),
            lr_item.getProductionType(),
        }));
    }
}// namespace ccl::parser