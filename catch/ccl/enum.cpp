#include <ccl/debug/debug_file.hpp>

using namespace ccl;

// NOLINTNEXTLINE
CCL_ENUM(TestStringEnum, std::string_view, ONE = "ONE", TWO = "TWO");

// NOLINTNEXTLINE
CCL_ENUM(TestIntEnum, int, ONE = 1, TWO = 2);

BOOST_AUTO_TEST_CASE(TestEnumWithInt)
{
    auto first_test_enum = TestIntEnum::ONE;
    auto second_test_enum = TestIntEnum::TWO;

    static_assert(std::is_nothrow_constructible_v<TestIntEnum>);
    static_assert(std::is_nothrow_move_assignable_v<TestIntEnum>);
    static_assert(std::is_nothrow_move_constructible_v<TestIntEnum>);

    BOOST_ASSERT(first_test_enum == 1);
    BOOST_ASSERT(second_test_enum == 2);

    std::swap(first_test_enum, second_test_enum);

    BOOST_ASSERT(first_test_enum == 2);
    BOOST_ASSERT(second_test_enum == 1);
}

BOOST_AUTO_TEST_CASE(TestEnumWithString)
{
    auto first_test_enum = TestStringEnum::ONE;
    auto second_test_enum = TestStringEnum::TWO;

    static_assert(std::is_nothrow_move_assignable_v<TestStringEnum>);
    static_assert(std::is_nothrow_move_constructible_v<TestStringEnum>);

    BOOST_ASSERT(first_test_enum == "ONE");
    BOOST_ASSERT(second_test_enum == "TWO");

    std::swap(first_test_enum, second_test_enum);

    BOOST_ASSERT(first_test_enum == "TWO");
    BOOST_ASSERT(second_test_enum == "ONE");

    first_test_enum = TestStringEnum::ONE;
    BOOST_ASSERT(first_test_enum == "ONE");
}
