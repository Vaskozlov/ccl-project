#ifndef CCL_PROJECT_TYPES_HPP
#define CCL_PROJECT_TYPES_HPP

#include <ccl/ccl.hpp>

namespace ccl::parser
{
    using State = std::size_t;
    using Symbol = std::size_t;

    template<typename T>
    using Stack = std::stack<T, std::vector<T>>;
}// namespace ccl::parser

#endif /* CCL_PROJECT_TYPES_HPP */
