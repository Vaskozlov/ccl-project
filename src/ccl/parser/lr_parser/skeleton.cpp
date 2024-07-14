#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/node_sequence.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr_parser.hpp"

namespace ccl::parser
{
    auto LrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> std::unique_ptr<ast::Node>
    {
        auto state_stack = isl::Vector<State>{0};
        auto nodes_stack = isl::Vector<std::unique_ptr<ast::Node>>{};
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
            case ParsingAction::SHIFT:
                nodes_stack.emplace_back(isl::makeUnique<ast::TokenNode>(word->getId(), *word));
                state_stack.push_back(action.getShiftingState());
                word = &tokenizer.yield();
                break;

            case ParsingAction::REDUCE:
                reduceAction(action, state_stack, nodes_stack);
                break;

            case ParsingAction::ACCEPT:
                return std::move(nodes_stack.back());

            default:
                isl::unreachable();
            }
        }
    }

    auto LrParser::reduceAction(
        const Action &action,
        isl::Vector<State> &state_stack,
        isl::Vector<std::unique_ptr<ast::Node>> &nodes_stack) const -> void
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