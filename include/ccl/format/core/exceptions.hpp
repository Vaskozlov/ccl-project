#ifndef CCL_PROJECT_EXCEPTIONS_HPP
#define CCL_PROJECT_EXCEPTIONS_HPP

#include <ccl/ccl.hpp>

namespace ccl::fmt
{
    CCL_EXCEPTION(BasicFormattingException, CclException);
    CCL_EXCEPTION(StringSplitterException, BasicFormattingException);
}// namespace ccl::fmt

#endif /* CCL_PROJECT_EXCEPTIONS_HPP */
