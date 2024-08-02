#ifndef CCL_PROJECT_RUNTIME_HPP
#define CCL_PROJECT_RUNTIME_HPP

#include <isl/coroutine/generator.hpp>
#include <isl/thread/id_generator.hpp>
#include <isl/thread/pool.hpp>

namespace ccl::runtime
{
    extern isl::thread::Pool GlobalThreadPool;

    template<typename T>
    auto async(isl::Task<T> task, isl::thread::Pool *pool = std::addressof(GlobalThreadPool))
        -> isl::AsyncTask<T>
    {
        return pool->submit(std::move(task));
    }
}// namespace ccl::runtime

#endif /* CCL_PROJECT_RUNTIME_HPP */
