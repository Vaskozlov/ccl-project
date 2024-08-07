#ifndef CCL_PROJECT_READER_OPTIONS_HPP
#define CCL_PROJECT_READER_OPTIONS_HPP

#include <ccl/ccl.hpp>

namespace ccl::parser::reader
{
    enum class ReaderOption : std::size_t
    {
        WARNINGS_ENABLE,
        SUGGESTIONS_ENABLE,
        IMMEDIATE_STOP_ON_ERROR,
    };
}

#endif /* CCL_PROJECT_READER_OPTIONS_HPP */
