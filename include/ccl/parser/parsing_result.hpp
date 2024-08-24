#ifndef CCL_PROJECT_PARSING_RESULT_HPP
#define CCL_PROJECT_PARSING_RESULT_HPP

#include <ccl/parser/ast/allocator.hpp>

namespace ccl::parser
{
    struct UnambiguousParsingResult
    {
        ast::SharedNode<> root{};
    };

    struct AmbiguousParsingResult
    {
        std::vector<ast::SharedNode<>> roots;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_PARSING_RESULT_HPP */
