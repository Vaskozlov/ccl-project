#include <cerberus/debug/debug_file.hpp>

// NOLINTNEXTLINE
CERBLIB_ENUM(TestStringEnum, std::string, ONE = "ONE", TWO = "TWO");

// NOLINTNEXTLINE
CERBLIB_ENUM(TestIntEnum, int, ONE = 1, TWO = 2);

CONSTEXPR_TEST
{
    auto first_test_enum = TestIntEnum::ONE;
    auto second_test_enum = TestIntEnum::TWO;

    assertTrue(std::is_nothrow_constructible_v<TestIntEnum>);
    assertTrue(std::is_nothrow_move_assignable_v<TestIntEnum>);
    assertTrue(std::is_nothrow_move_constructible_v<TestIntEnum>);

    assertEqual(first_test_enum, 1);
    assertEqual(second_test_enum, 2);

    std::swap(first_test_enum, second_test_enum);

    assertEqual(first_test_enum, 2);
    assertEqual(second_test_enum, 1);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto first_test_enum = TestStringEnum::ONE;
    auto second_test_enum = TestStringEnum::TWO;

    assertTrue(std::is_nothrow_constructible_v<TestStringEnum>);
    assertTrue(std::is_nothrow_move_assignable_v<TestStringEnum>);
    assertTrue(std::is_nothrow_move_constructible_v<TestStringEnum>);

    assertEqual(first_test_enum, "ONE");
    assertEqual(second_test_enum, "TWO");

    std::swap(first_test_enum, second_test_enum);

    assertEqual(first_test_enum, "TWO");
    assertEqual(second_test_enum, "ONE");

    first_test_enum = TestStringEnum::ONE;
    assertEqual(first_test_enum, "ONE");

    return {};
}
();
