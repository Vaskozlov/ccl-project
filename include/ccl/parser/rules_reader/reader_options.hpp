#ifndef CCL_PROJECT_READER_OPTIONS_HPP
#define CCL_PROJECT_READER_OPTIONS_HPP

namespace ccl::parser::reader
{
    enum class ReaderOption : u8
    {
        WARNINGS_ENABLE,
        SUGGESTIONS_ENABLE,
        IMMEDIATE_STOP_ON_ERROR,
    };
}

#endif /* CCL_PROJECT_READER_OPTIONS_HPP */
