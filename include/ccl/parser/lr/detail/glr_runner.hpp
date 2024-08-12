#ifndef CCL_PROJECT_GLR_RUNNER_HPP
#define CCL_PROJECT_GLR_RUNNER_HPP

#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/lr/glr_parser.hpp>
#include <forward_list>
#include <isl/thread/id_generator.hpp>

namespace ccl::parser::detail
{
    class GlrRunner;

    struct RunnersCommon
    {
        isl::SharedPtr<ast::TokenNode> word{};
        std::forward_list<GlrRunner> newRunnersInShiftState{};
        std::forward_list<GlrRunner> newRunnersInReduceState{};
        std::forward_list<GSStack> acceptedStacks{};
        std::forward_list<GSStack *> stacks{};
        std::function<std::string(SmallId)> idToStringConverter;
        const ankerl::unordered_dense::map<TableEntry, State> &gotoTable;
        const ankerl::unordered_dense::map<TableEntry, std::vector<Action>> &actionTable;
    };

    class GlrRunner
    {
    public:
        static isl::thread::IdGenerator<SmallId> runnerIdGenerator;

        Stack<State> stateStack{};
        GSStack nodeStack{};
        RunnersCommon *common{};
        SmallId inputLevel{};
        SmallId runnerId{};

        auto hostRunnersForAlternatives(const std::vector<Action> &actions) -> void;

        auto poll() -> ParsingAction;

        auto runAction(const Action &action) -> void;

        auto shiftAction(const Action &action) -> void;

        auto reduceAction(const Action &action) -> void;

        auto tryToJoin() -> bool;
    };
}// namespace ccl::parser::detail

#endif /* CCL_PROJECT_GLR_RUNNER_HPP */
