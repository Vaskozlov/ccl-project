#ifndef CCL_PROJECT_RUNTIME_HPP
#define CCL_PROJECT_RUNTIME_HPP

#include <isl/thread/pool.hpp>
#include <isl/coroutine/generator.hpp>

namespace ccl::runtime
{
    extern isl::thread::Pool GlobalThreadPool;
}

#endif /* CCL_PROJECT_RUNTIME_HPP */
