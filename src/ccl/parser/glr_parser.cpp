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
        const LrItem &start_item, Symbol epsilon_symbol, const isl::Set<Symbol> &grammar_symbols,
        const isl::Set<Symbol> &terminal_symbols, const GrammarRulesStorage &parser_rules)
    {
        auto parser_generator = LrParserGenerator(
            start_item, epsilon_symbol, grammar_symbols, terminal_symbols, parser_rules);

        gotoTable = std::move(parser_generator.getGotoTable());
        actionTable = parser_generator.getGlrActionTable();
    }

    auto GlrParser::parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
        -> isl::Vector<ast::ShNodePtr>
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

        do {
            runners_common.word = &tokenizer.yield();
            newWordIteration(runners_common, parsing_runners);
        } while (runners_common.word->getId() != 0);

        return std::move(runners_common.acceptedNodes);
    }
}// namespace ccl::parser