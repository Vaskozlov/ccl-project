#include <ccl/debug/debug_file.hpp>

using namespace ccl;

// NOLINTNEXTLINE
CCL_ENUM(TestIntEnum, int, ONE, TWO);

BOOST_AUTO_TEST_CASE(TestEnum)
{
    auto first_test_enum = TestIntEnum::ONE;
    auto second_test_enum = TestIntEnum::TWO;

    static_assert(std::is_nothrow_constructible_v<TestIntEnum>);
    static_assert(std::is_nothrow_move_assignable_v<TestIntEnum>);
    static_assert(std::is_nothrow_move_constructible_v<TestIntEnum>);

    BOOST_ASSERT(first_test_enum == 0);
    BOOST_ASSERT(second_test_enum == 1);

    std::swap(first_test_enum, second_test_enum);

    BOOST_ASSERT(first_test_enum == 1);
    BOOST_ASSERT(second_test_enum == 0);

    BOOST_ASSERT(TestIntEnum::contains(0));
    BOOST_ASSERT(TestIntEnum::contains(1));
}
