#ifndef CCL_PROJECT_LR_GSS_LEVEL_HPP
#define CCL_PROJECT_LR_GSS_LEVEL_HPP

#include <ccl/parser/ast/node_of_nodes.hpp>
#include <ccl/parser/lr/gss/node.hpp>

namespace ccl::parser::lr
{
    struct Level
    {
        std::vector<UniqueGssNodePtr> nonTerminals;
        UniqueGssNodePtr terminal;

        [[nodiscard]] auto
            findNonTerminal(SmallId parsing_state, ast::NodeOfNodes *value) const -> Node *;
    };
}// namespace ccl::parser::lr

#endif /* CCL_PROJECT_LR_GSS_LEVEL_HPP */
