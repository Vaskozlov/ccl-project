#include <ccl/runtime.hpp>

namespace ccl::runtime
{
    auto getGlobalThreadPool() -> isl::thread::Pool &
    {
        static isl::thread::Pool globalThreadPool{std::thread::hardware_concurrency() / 2};

        return globalThreadPool;
    }
} // namespace ccl::runtime
