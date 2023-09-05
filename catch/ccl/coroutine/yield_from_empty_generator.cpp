#include "ccl/debug/debug_file.hpp"
#include "ccl/generator.hpp"
#include <iostream>

ccl::Generator<size_t> someGenerator()
{
    co_yield ccl::as<size_t>(0);
}

TEST_CASE("YieldFromEmptyGenerator", "[Coroutine]")
{
    auto generator = someGenerator();

    REQUIRE(generator.yield() == 0);
    REQUIRE_THROWS(generator.yield());
}
