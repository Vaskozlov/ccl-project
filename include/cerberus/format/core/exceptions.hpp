#ifndef CERBERUS_PROJECT_EXCEPTIONS_HPP
#define CERBERUS_PROJECT_EXCEPTIONS_HPP

#include <cerberus/cerberus.hpp>

namespace cerb::fmt
{
    CERBLIB_EXCEPTION(BasicFormattingException, CerberusException);
    CERBLIB_EXCEPTION(StringSplitterException, BasicFormattingException);
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_EXCEPTIONS_HPP */
