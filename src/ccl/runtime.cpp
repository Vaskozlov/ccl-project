#include <ccl/runtime.hpp>

namespace ccl::runtime
{
    auto getGlobalThreadPool() -> isl::thread::Pool &
    {
#ifdef __EMSCRIPTEN__
        const std::size_t threads_count = 2;
#else
        const std::size_t threads_count = std::thread::hardware_concurrency() / 2;
#endif

        static isl::thread::Pool globalThreadPool{threads_count};

        return globalThreadPool;
    }
} // namespace ccl::runtime
