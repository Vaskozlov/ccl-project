#ifndef CCL_PROJECT_PARSING_RESULT_HPP
#define CCL_PROJECT_PARSING_RESULT_HPP

#include <ccl/parser/ast/node.hpp>

namespace ccl::parser
{
    struct UnambiguousParsingResult
    {
        ast::SharedNode<> root{};
        isl::string_view algorithmName;
    };

    struct AmbiguousParsingResult
    {
        std::vector<ast::SharedNode<>> roots{};
        isl::string_view algorithmName;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSING_RESULT_HPP */
