#include <cerberus/analysis/exception_accumulator.hpp>
#include <cerberus/debug/debug_file.hpp>

using namespace cerb;
using namespace cerb::analysis;

RUNTIME_TEST
{
    auto accumulator = ExceptionAccumulator<CerberusException>{};

    addError(&accumulator, CerberusException("CerberusException"));
    addWarning(&accumulator, LogicError("Logic error"));

    assertEqual(accumulator.getErrors().size(), 1_ZU);
    assertEqual(accumulator.getWarnings().size(), 1_ZU);

    return {};
}
();
