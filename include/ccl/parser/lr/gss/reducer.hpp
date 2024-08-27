#ifndef CCL_PROJECT_LR_GSS_REDUCER_HPP
#define CCL_PROJECT_LR_GSS_REDUCER_HPP

#include <ccl/parser/ast/non_terminal.hpp>
#include <ccl/parser/lr/gss/node.hpp>
#include <ccl/parser/table_entry.hpp>

namespace ccl::parser::lr
{
    class GSS;

    struct Reducer
    {
        std::function<ast::SharedNode<ast::NonTerminal>(std::vector<ast::SharedNode<>>)> reducer;
        const ankerl::unordered_dense::map<TableEntry, State> *gotoTable;
        GSS *gss;
        State production;
        SmallId inputLevel;

        auto
            reduce(SmallId pop_count, Node *node, std::vector<ast::SharedNode<>> arguments) -> void;
    };
}// namespace ccl::parser::lr

#endif /* CCL_PROJECT_LR_GSS_REDUCER_HPP */
