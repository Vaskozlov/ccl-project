#include "ccl/lazy.hpp"
#include "ccl/debug/debug_file.hpp"


BOOST_AUTO_TEST_CASE(LazyObjecyInitilizedGet)
{
    DEBUG_VAR int_object = ccl::Lazy(10);

    BOOST_ASSERT(int_object == 10);
    BOOST_ASSERT(int_object.get() == 10);
    BOOST_ASSERT(std::as_const(int_object).get() == 10);
}

BOOST_AUTO_TEST_CASE(LazyObjecyUninitilizedGet)
{
    DEBUG_VAR some_int = 42;
    DEBUG_VAR int_object = ccl::Lazy<int>([some_int]() {
        return some_int;
    });

    BOOST_ASSERT(int_object == some_int);
    BOOST_ASSERT(int_object.get() == some_int);
    BOOST_ASSERT(std::as_const(int_object).get() == some_int);
}