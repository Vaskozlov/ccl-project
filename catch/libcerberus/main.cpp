#include <boost/container/small_vector.hpp>
#include <fmt/format.h>

auto main() -> int
{
    boost::container::small_vector<int, 16>{};
    fmt::print("Hello, World!\n");
    
    return 0;
}