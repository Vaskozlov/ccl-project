#include <ccl/ccl.hpp>
#include <ccl/debug/debug_file.hpp>

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

TEST_CASE("TestAsOnInts", "[AsConversion]")
{
    static_assert(std::is_same_v<i64, decltype(as<i64>(10))>);
    REQUIRE(as<i64>(10) == 10);
}

TEST_CASE("TestAsOnFloats", "[AsConversion]")
{
    static_assert(std::is_same_v<f32, decltype(as<f32>(10))>);
    REQUIRE((as<f32>(10) - 10.0F) <= std::numeric_limits<f32>::epsilon());
}

TEST_CASE("TestAsOnPointer", "[AsConversion]")
{
    auto b = B{};

    auto *a = as<A *>(&b);
    static_assert(std::is_same_v<decltype(a), A *>);

    auto *b_from_a = as<B *>(a);
    static_assert(std::is_same_v<decltype(b_from_a), B *>);

    REQUIRE(a->getId() == 20);
    REQUIRE(b_from_a->getId() == 20);
}

TEST_CASE("TestAsOnReferences", "[AsConversion]")
{
    auto b = B{};

    auto &a = as<A &>(b);
    static_assert(std::is_same_v<decltype(a), A &>);

    auto &b_from_a = as<B &>(a);
    static_assert(std::is_same_v<decltype(b_from_a), B &>);

    REQUIRE(a.getId() == 20);
    REQUIRE(b_from_a.getId() == 20);
}

// NOLINTEND
