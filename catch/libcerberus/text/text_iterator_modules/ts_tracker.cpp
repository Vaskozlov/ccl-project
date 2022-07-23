#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator_modules/ts_tracker.hpp>

using namespace cerb::debug;
using namespace cerb::text::module;
using namespace cerb::integral_literals;
using namespace std::string_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto ts_tracker = TsTracker<char>{};
    ts_tracker.next(' ');
    ts_tracker.next('a');

    assertEqual(ts_tracker.get(), " ");

    ts_tracker.next('\t');
    ts_tracker.next(' ');
    assertEqual(ts_tracker.get(), "\t ");

    ts_tracker.next('\n');
    assertEqual(ts_tracker.get(), "\t ");

    ts_tracker.next('b');
    assertEqual(ts_tracker.get(), "");

    return {};
}
();
