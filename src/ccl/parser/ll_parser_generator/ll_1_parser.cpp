#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/ll/ll_1_parser.hpp>

namespace ccl::parser
{
    Ll1Parser::Ll1Parser(
        SmallId start_symbol, const GrammarStorage &grammar_storage,
        std::function<std::string(SmallId)> id_to_string_converter)
      : idToStringConverter{std::move(id_to_string_converter)}
      , storage{grammar_storage}
      , grammarGoalSymbol{start_symbol}
    {
        auto ll_generator = ll::LlParserGenerator{start_symbol, storage, idToStringConverter};
        table = ll_generator.createLl1Table();
    }

    auto Ll1Parser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer)
        -> std::pair<ast::Node *, isl::DynamicForwardList<ast::NodeOfNodes>>
    {
        const auto *word = std::addressof(tokenizer.yield());
        auto stack = Stack<ast::Node *>{};
        auto forward_list = isl::DynamicForwardList<ast::NodeOfNodes>{};
        auto *goal = forward_list.emplaceFront<ast::NodeOfNodes>(grammarGoalSymbol);

        stack.emplace(nullptr);
        stack.emplace(goal);

        while (true) {
            auto *focus = stack.top();

            if (focus == nullptr && word->getId() == 0) {
                return {goal, std::move(forward_list)};
            }

            if (storage.isTerminal(focus->getType())) {
                if (focus->getType() != word->getId()) {
                    return {nullptr, std::move(forward_list)};
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
                return {nullptr, std::move(forward_list)};
            }

            const auto *rule = rule_it->second;
            auto *focus_as_sequence = static_cast<ast::NodeOfNodes *>(focus);

            stack.pop();

            for (auto s : *rule | std::views::reverse) {
                if (s == storage.getEpsilon()) {
                    continue;
                }

                auto *built_node = std::add_pointer_t<ast::Node>{};

                if (storage.isTerminal(s)) {
                    built_node = focus_as_sequence->emplaceAfter<ast::TokenNode>(s);
                } else {
                    built_node = focus_as_sequence->emplaceAfter<ast::NodeOfNodes>(s);
                }

                focus_as_sequence->addNode(built_node);
                stack.emplace(focus_as_sequence->getNodes().back());
            }

            focus_as_sequence->reverse();
        }
    }
}// namespace ccl::parser
