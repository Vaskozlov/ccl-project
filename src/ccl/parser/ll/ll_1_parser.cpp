#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/ast/terminal.hpp>
#include <ccl/parser/ll/ll_1_parser.hpp>
#include <ccl/parser/parsing_result.hpp>

namespace ccl::parser
{
    Ll1Parser::Ll1Parser(
        const SmallId start_symbol, const GrammarStorage &grammar_storage,
        const std::function<std::string(SmallId)> &id_to_string_converter)
      : table{ll::Ll1ParserGenerator{grammar_storage, id_to_string_converter}.createLl1Table()}
      , idToStringConverter{id_to_string_converter}
      , storage{grammar_storage}
      , grammarGoalSymbol{start_symbol}
    {}

    auto Ll1Parser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) -> UnambiguousParsingResult
    {
        auto stack = Stack<ast::Node *>{};
        const auto *word = std::addressof(tokenizer.yield());
        auto goal = ast::SharedNode<ast::NonTerminal>(grammarGoalSymbol);
        auto parsing_result = UnambiguousParsingResult{.algorithmName = "LL(1)"};

        stack.emplace(nullptr);
        stack.emplace(goal.get());

        while (true) {
            auto *focus = stack.top();
            const auto token_type = word->getId();

            if (focus == nullptr && token_type == 0) {
                parsing_result.root = goal;
                return parsing_result;
            }

            if (focus == nullptr) {
                return parsing_result;
            }

            const auto focus_type = focus->getType();

            if (storage.isTerminal(focus_type)) {
                if (focus_type != token_type) {
                    return parsing_result;
                }

                dynamic_cast<ast::Terminal *>(focus)->setToken(*word);
                stack.pop();
                word = std::addressof(tokenizer.yield());
                continue;
            }

            auto rule_it = table.find({
                .state = focus_type,
                .symbol = token_type,
            });

            if (rule_it == table.end()) {
                return parsing_result;
            }

            const auto *rule = rule_it->second;
            auto *focus_as_sequence = static_cast<ast::NonTerminal *>(focus);

            stack.pop();

            for (auto s : *rule | std::views::reverse) {
                if (s == storage.getEpsilon()) {
                    continue;
                }

                auto built_node = ast::SharedNode<>{};

                if (storage.isTerminal(s)) {
                    built_node = ast::SharedNode<ast::Terminal>(s);
                } else {
                    built_node = ast::SharedNode<ast::NonTerminal>(s);
                }

                stack.emplace(built_node.get());
                focus_as_sequence->addNode(std::move(built_node));
            }

            focus_as_sequence->reverse();
        }
    }
}// namespace ccl::parser
