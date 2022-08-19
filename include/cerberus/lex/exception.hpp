#ifndef CERBERUS_PROJECT_LEX_EXCEPTION_HPP
#define CERBERUS_PROJECT_LEX_EXCEPTION_HPP

#include <cerberus/text/iterator_exception.hpp>

namespace cerb::lex
{
    CERBLIB_EXCEPTION(LexicalAnalysisException, text::TextIteratorException);
}

#endif /* CERBERUS_PROJECT_LEX_EXCEPTION_HPP */
