#ifndef CCL_PROJECT_LR_GSS_REDUCER_HPP
#define CCL_PROJECT_LR_GSS_REDUCER_HPP

#include <ccl/parser/table_entry.hpp>

namespace ccl::parser::lr
{
    class GSS;

    struct Reducer
    {
        const ankerl::unordered_dense::map<TableEntry, State> *gotoTable;
        GSS *gss;
        State production;
        SmallId inputLevel;

        auto reduce(SmallId pop_count, Node *node, ast::SmallVectorOfNodes arguments) -> void;
    };
}// namespace ccl::parser::lr

#endif /* CCL_PROJECT_LR_GSS_REDUCER_HPP */
