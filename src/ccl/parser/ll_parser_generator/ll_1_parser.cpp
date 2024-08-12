#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/ll/ll_1_parser.hpp>

namespace ccl::parser::ll
{
    LL1::LL1(
        SmallId start_symbol, const GrammarRulesStorage &grammar_storage,
        std::function<std::string(SmallId)> id_to_string_converter)
      : idToStringConverter{std::move(id_to_string_converter)}
      , storage{grammar_storage}
    {
        auto ll_generator = LlParserGenerator{start_symbol, storage, idToStringConverter};
        table = std::move(ll_generator.table);
    }

    auto LL1::parse(Symbol start, typename ccl::lexer::LexicalAnalyzer::Tokenizer &tokenizer)
        -> ast::ShNodePtr
    {
        const auto *word = std::addressof(tokenizer.yield());
        auto stack = Stack<ast::ShNodePtr>{};
        auto goal = isl::makeShared<ast::ShNodeSequence>(start);

        stack.emplace(nullptr);
        stack.emplace(goal);

        auto old_focus = stack.top();
        auto focus = stack.top();

        fmt::println("{}", idToStringConverter(focus->getType()));
        fmt::println("{}", idToStringConverter(word->getId()));

        while (true) {
            if (focus == nullptr && word->getId() == 0) {
                return std::move(goal);
            }

            if (storage.isTerminal(focus->getType())) {
                if (focus->getType() == word->getId()) {
                    dynamic_cast<ast::ShNodeSequence *>(old_focus.get())
                        ->addNode(isl::makeShared<ast::TokenNode>(
                            focus->getType(), tokenizer.getCurrentToken()));
                    fmt::println("{}:", idToStringConverter(stack.top()->getType()));
                    stack.pop();
                    word = std::addressof(tokenizer.yield());
                    fmt::println(
                        "{} - {}", idToStringConverter(old_focus->getType()),
                        idToStringConverter(word->getId()));
                } else {
                    return nullptr;
                }
            } else {
                const auto *rule = table.at({focus->getType(), word->getId()});
                auto *focus_as_sequence = dynamic_cast<ast::ShNodeSequence *>(focus.get());
                fmt::println(
                    "{}: {}", idToStringConverter(focus->getType()),
                    std::views::transform(*rule, idToStringConverter));

                stack.pop();

                for (auto s : *rule | std::views::reverse) {
                    if (s != storage.getEpsilon()) {
                        stack.emplace(isl::makeUnique<ast::ShNodeSequence>(s));
                        focus_as_sequence->addNode(stack.top());
                    }
                }

                focus_as_sequence->reverse();
            }

            old_focus = std::move(focus);
            focus = stack.top();
        }
    }
}// namespace ccl::parser::ll
