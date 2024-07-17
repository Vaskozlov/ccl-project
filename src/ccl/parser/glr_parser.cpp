#include "ccl/parser/lr/glr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr/detail/glr_runner.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    using namespace detail;
    using enum ccl::parser::ParsingAction;

    static auto pollRunners(std::list<Runner> &parsing_runners) -> std::list<Runner>
    {
        auto running_runners = std::list<Runner>{};

        for (auto &runner : parsing_runners) {
            switch (runner.poll()) {
            case ParsingAction::SHIFT:
            case ParsingAction::REDUCE:
                running_runners.emplace_back(std::move(runner));
                break;

            default:
                break;
            }
        }

        return running_runners;
    }

    static auto
        newWordIteration(detail::RunnersCommon &common, std::list<Runner> &parsing_runners) -> void
    {
        parsing_runners = pollRunners(parsing_runners);

        while (!common.newRunnersInReduceState.empty()) {
            auto runners_in_reduce_state_copy = std::move(common.newRunnersInReduceState);
            // after move object is in unknown state
            common.newRunnersInReduceState.clear();

            parsing_runners.splice(parsing_runners.end(), common.newRunnersInShiftState);
            parsing_runners.splice(
                parsing_runners.end(), pollRunners(runners_in_reduce_state_copy));
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

        auto parsing_runners = std::list<Runner>{};

        parsing_runners.emplace_back(Runner{
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