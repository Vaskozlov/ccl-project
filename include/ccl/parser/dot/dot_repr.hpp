#ifndef CCL_PROJECT_GRAPH_HPP
#define CCL_PROJECT_GRAPH_HPP

#include <ccl/parser/ast/node.hpp>
#include <filesystem>
#include <isl/gss.hpp>

namespace ccl::parser::dot
{
    auto createDotRepresentation(
        const std::vector<const ast::Node *> &roots,
        std::function<std::string(SmallId)>
            node_type_to_string) -> std::string;
}// namespace ccl::parser::dot

#endif /* CCL_PROJECT_GRAPH_HPP */
