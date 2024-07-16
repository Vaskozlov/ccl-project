#ifndef CCL_PROJECT_GLR_RUNNER_HPP
#define CCL_PROJECT_GLR_RUNNER_HPP

#include <ccl/parser/lr/glr_parser.hpp>

namespace ccl::parser::detail
{
    using ListOfPendingAlternatives =
        std::list<isl::Pair<ParsingAction, isl::Generator<ParsingAction>>>;

    struct RunnersCommon
    {
        ListOfPendingAlternatives pendingAlternatives;
        isl::Vector<ast::ShNodePtr> acceptedNodes;
        const isl::Map<TableEntry, State> &gotoTable;
        const isl::Map<TableEntry, isl::Vector<Action>> &actionTable;
        const lexer::Token *word;
    };

    class Runner
    {
    public:
        isl::WeakStack<State> stateStack;
        isl::WeakStack<ast::ShNodePtr> nodesStack;
        RunnersCommon &common;

        auto hostNewRunnersIfMoreThanOneAction(const isl::Vector<Action> &actions) -> void;

        auto runAction(const Action &action) -> void;

        auto reduceAction(const Action &action) -> void;
    };

    auto constructRunnerPoller(Runner runner) -> isl::Generator<ParsingAction>;
}// namespace ccl::parser::detail

#endif /* CCL_PROJECT_GLR_RUNNER_HPP */
