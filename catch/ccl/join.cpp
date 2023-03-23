#include "ccl/join.hpp"
#include "ccl/debug/debug_file.hpp"

using namespace ccl;
using namespace string_view_literals;
using namespace std::string_view_literals;

TEST_CASE("JoinInts", "[Join]")
{
    DEBUG_VAR vector_of_int = Vector<int>{10, 20, 30};// NOLINT

    REQUIRE(
        ccl::join(
            vector_of_int,
            [](auto &&elem) {
                return fmt::to_string(elem);
            },
            ", ") == "10, 20, 30");

    REQUIRE(ccl::join(vector_of_int, ", ") == "10, 20, 30");
}
