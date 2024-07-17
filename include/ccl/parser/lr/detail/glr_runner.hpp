#ifndef CCL_PROJECT_GLR_RUNNER_HPP
#define CCL_PROJECT_GLR_RUNNER_HPP

#include <ccl/parser/lr/glr_parser.hpp>

namespace ccl::parser::detail
{
    class Runner;

    struct RunnersCommon
    {
        std::list<Runner> newRunnersInShiftState;
        std::list<Runner> newRunnersInReduceState;
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

        auto poll() -> ParsingAction;

        auto hostNewRunnersIfMoreThanOneAction(const isl::Vector<Action> &actions) -> void;

        auto runAction(const Action &action) -> void;

        auto reduceAction(const Action &action) -> void;
    };
}// namespace ccl::parser::detail

#endif /* CCL_PROJECT_GLR_RUNNER_HPP */
