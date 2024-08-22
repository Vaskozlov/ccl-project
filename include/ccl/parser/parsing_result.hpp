#ifndef CCL_PROJECT_PARSING_RESULT_HPP
#define CCL_PROJECT_PARSING_RESULT_HPP

#include <ccl/parser/nodes_lifetime_manager.hpp>

namespace ccl::parser
{
    struct UnambiguousParsingResult
    {
        std::unique_ptr<NodesLifetimeManager> nodesLifetimeManager =
            std::make_unique<NodesLifetimeManager>();
        ast::Node *root{};
    };

    struct AmbiguousParsingResult
    {
        std::unique_ptr<NodesLifetimeManager> nodesLifetimeManager =
            std::make_unique<NodesLifetimeManager>();
        std::vector<ast::Node *> roots;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSING_RESULT_HPP */
