#include <cerberus/coroutine/generator.hpp>
#include <cerberus/debug/debug_file.hpp>

// NOLINTBEGIN

auto intGenerator() -> cerb::coro::Generator<int>
{
    int value = 0;

    while (true) {
        if (value == 11) {
            break;
        }

        co_yield value;
        ++value;
    }
}

RUNTIME_TEST
{
    auto gen = intGenerator();

    for (auto i = 0; i != 10; ++i) {
        assertEqual(i, gen());
    }

    assertEqual(gen(), 10);

    return {};
}
();

// NOLINTEND
