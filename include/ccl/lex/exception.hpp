#ifndef CCL_PROJECT_LEX_EXCEPTION_HPP
#define CCL_PROJECT_LEX_EXCEPTION_HPP

#include <ccl/text/iterator_exception.hpp>

namespace ccl::lex
{
    CCL_EXCEPTION(LexicalAnalysisException, text::TextIteratorException);
}

#endif /* CCL_PROJECT_LEX_EXCEPTION_HPP */
