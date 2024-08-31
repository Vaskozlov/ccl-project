#ifndef CCL_PROJECT_TYPES_HPP
#define CCL_PROJECT_TYPES_HPP

#include <ccl/ccl.hpp>
#include <stack>

namespace ccl::parser
{
    using State = SmallId;
    using Symbol = SmallId;

    template<typename T>
    using Stack = std::stack<T, std::vector<T>>;
}// namespace ccl::parser

#endif /* CCL_PROJECT_TYPES_HPP */
