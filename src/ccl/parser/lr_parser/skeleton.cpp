#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/node_sequence.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr_parser.hpp"

namespace ccl::parser
{
    auto LrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) -> std::unique_ptr<ast::Node>
    {
        auto state_stack = isl::Vector<Id>{0};
        auto token_stack = isl::Vector<std::unique_ptr<ast::Node>>{};
        const auto *word = &tokenizer.yield();

        while (true) {
            const auto state = state_stack.back();
            const auto entry = TableEntry{state, word->getId()};

            if (!actionTable.contains(entry)) {
                return nullptr;
            }

            const auto &action = actionTable.at(entry);

            switch (action.getParsingAction()) {
            case ParsingAction::SHIFT:
                token_stack.emplace_back(isl::makeUnique<ast::TokenNode>(word->getId(), *word));
                state_stack.push_back(action.getShiftingState());
                word = &tokenizer.yield();
                break;

            case ParsingAction::REDUCE: {
                const auto &lr_item = action.getReducingItem();
                auto reduced_item =
                    isl::makeUnique<ast::NodeSequence>(isl::as<Id>(lr_item.getProductionType()));

                for (std::size_t i = 0; i != lr_item.length(); ++i) {
                    reduced_item->addNode(std::move(token_stack.back()));
                    token_stack.pop_back();
                    state_stack.pop_back();
                }

                reduced_item->reverse();
                token_stack.push_back(std::move(reduced_item));
                state_stack.push_back(
                    gotoTable.at({state_stack.back(), lr_item.getProductionType()}));

                break;
            }

            case ParsingAction::ACCEPT:
                return std::move(token_stack.back());

            default:
                std::unreachable();
            }
        }
    }
}// namespace ccl::parser