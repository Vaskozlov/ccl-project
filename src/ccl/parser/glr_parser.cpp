#include "ccl/parser/lr/glr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr/detail/glr_runner.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"
#include <algorithm>

namespace ccl::parser
{
    using namespace detail;
    using enum ccl::parser::ParsingAction;

    static auto pollRunners(std::forward_list<Runner> &parsing_runners) -> void
    {
        std::erase_if(parsing_runners, [](Runner &runner) {
            const auto polling_result = runner.poll();
            return polling_result == ACCEPT || polling_result == FAILED;
        });
    }

    static auto newWordIteration(
        detail::RunnersCommon &common, std::forward_list<Runner> &parsing_runners) -> void
    {
        pollRunners(parsing_runners);

        while (!common.newRunnersInReduceState.empty()) {
            auto runners_in_reduce_state_copy = std::move(common.newRunnersInReduceState);
            // after move object is in unknown state
            common.newRunnersInReduceState.clear();

            parsing_runners.splice_after(
                parsing_runners.before_begin(), common.newRunnersInShiftState);

            pollRunners(runners_in_reduce_state_copy);

            parsing_runners.splice_after(
                parsing_runners.before_begin(), runners_in_reduce_state_copy);
        }
    }

    GlrParser::GlrParser(
        const LrItem &start_item, Symbol epsilon_symbol, const GrammarRulesStorage &parser_rules)
    {
        auto parser_generator =
            LrParserGenerator(start_item, epsilon_symbol, parser_rules, nullptr);

        gotoTable = std::move(parser_generator.getGotoTable());
        actionTable = parser_generator.getGlrActionTable();
    }

    auto GlrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> std::vector<ast::ShNodePtr>
    {
        auto runners_common = detail::RunnersCommon{
            .newRunnersInShiftState = {},
            .newRunnersInReduceState = {},
            .acceptedNodes = {},
            .gotoTable = gotoTable,
            .actionTable = actionTable,
            .word = nullptr,
        };

        auto parsing_runners = std::forward_list<Runner>{};

        parsing_runners.emplace_front(Runner{
            .stateStack = {0},
            .nodesStack = {},
            .common = runners_common,
        });

        const auto *new_token = isl::as<lexer::Token *>(nullptr);

        do {
            new_token = &tokenizer.yield();
            runners_common.word = isl::makeShared<ast::TokenNode>(new_token->getId(), *new_token);
            newWordIteration(runners_common, parsing_runners);
        } while (new_token->getId() != 0);

        return std::move(runners_common.acceptedNodes);
    }
}// namespace ccl::parser