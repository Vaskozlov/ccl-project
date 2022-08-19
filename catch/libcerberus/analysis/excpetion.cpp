#include <boost/test/unit_test.hpp>
#include <cerberus/analysis/exception_accumulator.hpp>

using namespace cerb;
using namespace cerb::analysis;

BOOST_AUTO_TEST_CASE(AnalysisExceptionAccumulator)
{
    auto accumulator = ExceptionAccumulator<CerberusException>{};

    addError(&accumulator, CerberusException("CerberusException"));
    addWarning(&accumulator, LogicError("Logic error"));

    BOOST_CHECK_EQUAL(accumulator.getErrors().size(), 1);
    BOOST_CHECK_EQUAL(accumulator.getWarnings().size(), 1);
}
