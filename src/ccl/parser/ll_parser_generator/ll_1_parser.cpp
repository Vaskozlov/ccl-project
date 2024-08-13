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

    auto Ll1Parser::parse(typename ccl::lexer::LexicalAnalyzer::Tokenizer &tokenizer)
        -> ast::UnNodePtr
    {
        const auto *word = std::addressof(tokenizer.yield());
        auto stack = Stack<ast::Node *>{};
        auto goal = isl::makeUnique<ast::UnNodeSequence>(grammarGoalSymbol);

        stack.emplace(nullptr);
        stack.emplace(goal.get());

        while (true) {
            auto *focus = stack.top();

            if (focus == nullptr && word->getId() == 0) {
                return std::move(goal);
            }

            if (storage.isTerminal(focus->getType())) {
                if (focus->getType() != word->getId()) {
                    return nullptr;
                }

                static_cast<ast::TokenNode *>(focus)->setToken(*word);
                stack.pop();
                word = std::addressof(tokenizer.yield());
                continue;
            }

            const auto *rule = table.at({focus->getType(), word->getId()});
            auto *focus_as_sequence = static_cast<ast::UnNodeSequence *>(focus);
            stack.pop();

            for (auto s : *rule | std::views::reverse) {
                if (s == storage.getEpsilon()) {
                    continue;
                }

                if (storage.isTerminal(s)) {
                    focus_as_sequence->addNode(isl::makeUnique<ast::TokenNode>(s));
                } else {
                    focus_as_sequence->addNode(isl::makeUnique<ast::UnNodeSequence>(s));
                }

                stack.emplace(focus_as_sequence->getNodes().back().get());
            }

            focus_as_sequence->reverse();
        }
    }
}// namespace ccl::parser
