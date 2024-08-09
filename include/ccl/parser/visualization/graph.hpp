#ifndef CCL_PROJECT_GRAPH_HPP
#define CCL_PROJECT_GRAPH_HPP

#include <ccl/parser/ast/node.hpp>
#include <filesystem>

namespace ccl::parser::visualization
{
    auto visualize(
        const std::filesystem::path &resulted_file_path, const ast::Node *root,
        std::function<std::string(SmallId)> node_type_to_string) -> void;
}

#endif /* CCL_PROJECT_GRAPH_HPP */
