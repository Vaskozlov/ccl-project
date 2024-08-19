#include "ccl/parser/lr/glr_parser.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/lr/detail/glr_runner.hpp"
#include "ccl/parser/lr/detail/lr_parser_generator.hpp"

namespace ccl::parser
{
    using namespace detail;
    using enum ParsingAction;

    static auto joinStacks(std::forward_list<GSStack> &accepted_stacks) -> GSStack
    {
        auto front_stack = std::move(accepted_stacks.front());

        for (auto &stack : accepted_stacks | std::views::drop(1)) {
            front_stack.merge(stack);
        }

        return front_stack;
    }

    static auto
    pollRunners(RunnersCommon&common, std::forward_list<GlrRunner>&parsing_runners) -> void {
        std::erase_if(parsing_runners, [&common](GlrRunner &runner) {
            const auto polling_result = runner.poll();
            const auto *stack_ptr = std::addressof(runner.nodeStack);

            if (polling_result == ACCEPT) {
                std::erase(common.stacks, stack_ptr);
                auto &succeed_stack =
                    common.acceptedStacks.emplace_front(std::move(runner.nodeStack));
                common.stacks.emplace_front(std::addressof(succeed_stack));
            }

            return polling_result != SHIFT && polling_result != REDUCE;
        });
    }

    static auto newWordIteration(
        RunnersCommon&common,
        std::forward_list<GlrRunner> &parsing_runners) -> void
    {
        pollRunners(common, parsing_runners);

        while (!common.newRunnersInReduceState.empty()) {
            auto runners_in_reduce_state_copy = std::move(common.newRunnersInReduceState);
            // after move object is in unknown state
            common.newRunnersInReduceState.clear();

            pollRunners(common, runners_in_reduce_state_copy);

            parsing_runners.splice_after(
                parsing_runners.before_begin(), runners_in_reduce_state_copy);
        }

        parsing_runners.splice_after(parsing_runners.before_begin(), common.newRunnersInShiftState);
    }

    GlrParser::GlrParser(
        const GrammarSlot &start_item,
        Symbol epsilon_symbol,
        const GrammarStorage &parser_rules,
        std::function<std::string(SmallId)>
            id_to_string_converter)
      : idToStringConverter{std::move(id_to_string_converter)}
    {
        auto parser_generator =
            LrParserGenerator(start_item, epsilon_symbol, parser_rules, idToStringConverter);

        gotoTable = std::move(parser_generator.getGotoTable());
        actionTable = parser_generator.getGlrActionTable();
    }

    auto GlrParser::parse(lexer::LexicalAnalyzer::Tokenizer&tokenizer) const
        -> std::pair<GSStack, isl::DynamicForwardList<ast::Node>> {
        auto forward_list = isl::DynamicForwardList<ast::Node>{};
        auto common = RunnersCommon{
            .idToStringConverter = idToStringConverter,
            .gotoTable = gotoTable,
            .actionTable = actionTable,
        };

        auto parsing_runners = std::forward_list<GlrRunner>{};

        parsing_runners.emplace_front(
            GlrRunner{
                .common = std::addressof(common),
            });

        common.stacks.emplace_front(std::addressof(parsing_runners.front().nodeStack));
        parsing_runners.front().stateStack.emplace(0);

        const auto *new_token = isl::as<lexer::Token *>(nullptr);

        do {
            new_token = &tokenizer.yield();
            common.word = forward_list.emplaceFront<ast::TokenNode>(new_token->getId(), *new_token);
            newWordIteration(common, parsing_runners);
        } while (new_token->getId() != 0);

        return {joinStacks(common.acceptedStacks), std::move(forward_list)};
    }
} // namespace ccl::parser
