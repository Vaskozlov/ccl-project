#include "ccl/parser/lr/glr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/node_sequence.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr/deatil/glr_runner.hpp"
#include "ccl/parser/lr/lr_parser_generator.hpp"

namespace ccl::parser
{
    using enum ccl::parser::ParsingAction;
    using ListOfPollers = std::list<isl::Generator<ParsingAction>>;

    static auto pollAlternatives(ListOfPollers &parsing_alternatives) -> void
    {
        for (auto &alternative : parsing_alternatives) {
            CCL_REPEAT_WHILE(alternative.yield() == REDUCE)
        }
    }

    static auto pollAlternativesIfNotInShift(
        detail::ListOfPendingAlternatives &parsing_alternatives) -> void
    {
        for (auto &[action, alternative] : parsing_alternatives) {
            if (action == SHIFT) {
                continue;
            }

            CCL_REPEAT_WHILE(alternative.yield() == REDUCE)
        }
    }

    static auto
        newWordIteration(detail::RunnersCommon &common, ListOfPollers &parsing_alternatives) -> void
    {
        pollAlternatives(parsing_alternatives);

        while (!common.pendingAlternatives.empty()) {
            auto pending_copy = std::move(common.pendingAlternatives);
            common.pendingAlternatives.clear();

            pollAlternativesIfNotInShift(pending_copy);

            for (auto &&[action, alternative] : pending_copy) {
                parsing_alternatives.emplace_back(std::move(alternative));
            }
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
            .pendingAlternatives = {},
            .acceptedNodes = {},
            .gotoTable = gotoTable,
            .actionTable = actionTable,
            .word = nullptr,
        };

        auto parsing_alternatives = ListOfPollers{};

        parsing_alternatives.emplace_back(detail::constructRunnerPoller(detail::Runner{
            .stateStack = isl::WeakStack<State>{0},
            .nodesStack = {},
            .common = runners_common,
        }));

        do {
            runners_common.word = &tokenizer.yield();
            newWordIteration(runners_common, parsing_alternatives);
        } while (runners_common.word->getId() != 0);

        return std::move(runners_common.acceptedNodes);
    }
}// namespace ccl::parser