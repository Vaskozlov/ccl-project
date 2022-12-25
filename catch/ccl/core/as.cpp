#include <boost/test/unit_test_suite.hpp>
#include <ccl/core/as.hpp>
#include <ccl/debug/debug_file.hpp>
#include <limits>
#include <type_traits>

using namespace ccl;

// NOLINTBEGIN

struct A
{
    A() = default;

    virtual ~A() = default;

    virtual auto getId() const -> int
    {
        return 10;
    }
};

struct B : A
{
    auto getId() const -> int override
    {
        return 20;
    }
};

BOOST_AUTO_TEST_SUITE(AsConvertion)

BOOST_AUTO_TEST_CASE(TestAsOnInts)
{
    static_assert(std::is_same_v<i64, decltype(as<i64>(10))>);
    BOOST_ASSERT(as<i64>(10) == 10);
}

BOOST_AUTO_TEST_CASE(TestAsOnFloats)
{
    static_assert(std::is_same_v<f32, decltype(as<f32>(10))>);
    BOOST_ASSERT((as<f32>(10) - 10.0F) <= std::numeric_limits<f32>::epsilon());
}

BOOST_AUTO_TEST_CASE(TestAsOnPointer)
{
    auto b = B{};

    auto *a = as<A *>(&b);
    static_assert(std::is_same_v<decltype(a), A *>);

    auto *b_from_a = as<B *>(a);
    static_assert(std::is_same_v<decltype(b_from_a), B *>);

    BOOST_ASSERT(a->getId() == 20);
    BOOST_ASSERT(b_from_a->getId() == 20);
}

BOOST_AUTO_TEST_CASE(TestAsOnReferences)
{
    auto b = B{};

    auto &a = as<A &>(b);
    static_assert(std::is_same_v<decltype(a), A &>);

    auto &b_from_a = as<B &>(a);
    static_assert(std::is_same_v<decltype(b_from_a), B &>);

    BOOST_ASSERT(a.getId() == 20);
    BOOST_ASSERT(b_from_a.getId() == 20);
}

// NOLINTEND

BOOST_AUTO_TEST_SUITE_END()
