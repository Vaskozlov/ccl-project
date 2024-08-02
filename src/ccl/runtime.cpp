#include <ccl/runtime.hpp>

namespace ccl::runtime
{
    isl::thread::Pool GlobalThreadPool{std::thread::hardware_concurrency() / 2};
}
