#include <ccl/analysis/exception_accumulator.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;
using namespace analysis;

BOOST_AUTO_TEST_CASE(AnalysisExceptionAccumulator)
{
    auto accumulator = ExceptionAccumulator<CclException>{};

    addError(&accumulator, CclException("BasicCclException"));
    addWarning(&accumulator, LogicError("Logic error"));

    BOOST_CHECK_EQUAL(accumulator.getErrors().size(), 1);
    BOOST_CHECK_EQUAL(accumulator.getWarnings().size(), 1);
}
