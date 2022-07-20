#include <cerberus/analysis/exception.hpp>
#include <cerberus/debug/debug.hpp>

using namespace cerb;
using namespace cerb::debug;
using namespace cerb::analysis;
using namespace cerb::integral_literals;

VECTOR_TEST
{
    auto accumulator = ExceptionAccumulator<CerberusException>{};

    accumulator.addError(CerberusException("CerberusException"));
    accumulator.addWarning(LogicError("Logic error"));

    assertEqual(accumulator.getErrors().size(), 1_ZU);
    assertEqual(accumulator.getWarnings().size(), 1_ZU);

    return {};
};
