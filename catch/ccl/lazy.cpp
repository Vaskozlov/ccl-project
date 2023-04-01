#include "ccl/lazy.hpp"
#include "ccl/debug/debug_file.hpp"

TEST_CASE("LazyObjectInitializedGet", "[Lazy]")
{
    DEBUG_VAR int_object = ccl::toLazy(10);// NOLINT

    REQUIRE(int_object.get() == 10);
    REQUIRE(std::as_const(int_object).get() == 10);
}

TEST_CASE("LazyObjectUninitializedGet", "[Lazy]")
{
    DEBUG_VAR some_int = 42;// NOLINT
    DEBUG_VAR int_object = ccl::toLazy([some_int]() {
        return some_int;
    });

    REQUIRE(int_object.get() == some_int);
    REQUIRE(std::as_const(int_object).get() == some_int);
}