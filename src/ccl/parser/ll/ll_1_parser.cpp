#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/ll/ll_1_parser.hpp>
#include <ccl/parser/parsing_result.hpp>

namespace ccl::parser
{
    Ll1Parser::Ll1Parser(
        SmallId start_symbol, const GrammarStorage &grammar_storage,
        const std::function<std::string(SmallId)> &id_to_string_converter)
      : idToStringConverter{id_to_string_converter}
      , storage{grammar_storage}
      , grammarGoalSymbol{start_symbol}
    {
        const auto ll_generator = ll::LlParserGenerator{start_symbol, storage, idToStringConverter};
        table = ll_generator.createLl1Table();
    }

    auto Ll1Parser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) -> UnambiguousParsingResult
    {
        const auto *word = std::addressof(tokenizer.yield());
        auto stack = Stack<ast::Node *>{};
        auto parsing_result = UnambiguousParsingResult{};
        auto goal = ast::SharedNode<ast::NodeOfNodes>(grammarGoalSymbol);

        stack.emplace(nullptr);
        stack.emplace(goal.get());

        while (true) {
            auto *focus = stack.top();

            if (focus == nullptr && word->getId() == 0) {
                parsing_result.root = goal;
                return parsing_result;
            }

            if (storage.isTerminal(focus->getType())) {
                if (focus->getType() != word->getId()) {
                    return parsing_result;
                }

                dynamic_cast<ast::TokenNode *>(focus)->setToken(*word);
                stack.pop();
                word = std::addressof(tokenizer.yield());
                continue;
            }

            const auto entry = TableEntry{
                .state = focus->getType(),
                .symbol = word->getId(),
            };

            auto rule_it = table.find(entry);

            if (rule_it == table.end()) {
                return parsing_result;
            }

            const auto *rule = rule_it->second;
            auto *focus_as_sequence = static_cast<ast::NodeOfNodes *>(focus);

            stack.pop();

            for (auto s : *rule | std::views::reverse) {
                if (s == storage.getEpsilon()) {
                    continue;
                }

                auto built_node = ast::SharedNode<>{};

                if (storage.isTerminal(s)) {
                    built_node = ast::SharedNode<ast::TokenNode>(s);
                } else {
                    built_node = ast::SharedNode<ast::NodeOfNodes>(s);
                }

                focus_as_sequence->addNode(std::move(built_node));
                stack.emplace(focus_as_sequence->getNodes().back().get());
            }

            focus_as_sequence->reverse();
        }
    }
}// namespace ccl::parser