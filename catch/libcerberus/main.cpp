#include <boost/container/small_vector.hpp>
#include <fmt/format.h>

auto main() -> int
{
    [[maybe_unused]] boost::container::small_vector<int, 4> _{};
    fmt::print("Hello, World!\n");

    return 0;
}