#ifndef CCL_PROJECT_RUNTIME_HPP
#define CCL_PROJECT_RUNTIME_HPP

#include <isl/thread/lockfree/spsc_pipe.hpp>
#include <isl/thread/pool.hpp>

namespace ccl::runtime
{
    inline isl::thread::Pool GlobalThreadPool{std::thread::hardware_concurrency() / 2};

    template<typename T>
    auto async(isl::Task<T> task, isl::thread::Pool *pool = std::addressof(GlobalThreadPool))
        -> isl::AsyncTask<T>
    {
        return pool->submit(std::move(task));
    }

    template<typename T, std::size_t N = 512>
    using Pipe = isl::thread::lock_free::SpscPipe<T, N>;
}// namespace ccl::runtime

#endif /* CCL_PROJECT_RUNTIME_HPP */
