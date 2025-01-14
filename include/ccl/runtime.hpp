#ifndef CCL_PROJECT_RUNTIME_HPP
#define CCL_PROJECT_RUNTIME_HPP

#include <isl/thread/async_task.hpp>
#include <isl/thread/lockfree/spsc_pipe.hpp>
#include <isl/thread/pool.hpp>

namespace ccl::runtime
{
    auto getGlobalThreadPool() -> isl::thread::Pool &;

    template <typename T>
    auto async(isl::Task<T> task, isl::thread::Pool *pool = std::addressof(getGlobalThreadPool()))
        -> isl::AsyncTask<T>
    {
        return pool->async(std::move(task));
    }

    template <typename T, std::size_t N = 512>
    using Pipe = isl::thread::lock_free::SpscPipe<T, N>;
} // namespace ccl::runtime

#endif /* CCL_PROJECT_RUNTIME_HPP */
