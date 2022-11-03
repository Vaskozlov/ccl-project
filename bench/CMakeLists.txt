enable_testing()

file(
        GLOB_RECURSE
        CCL_BENCH_OBJECTS
        "bench/*.cpp"
)

add_executable(
        ccl-bench
        ${CCL_BENCH_OBJECTS}
)

add_dependencies(ccl-bench fmt::fmt)
add_dependencies(ccl-bench benchmark::benchmark)

target_link_libraries(
        ccl-bench
        fmt::fmt
)

target_link_libraries(
        ccl-bench
        benchmark::benchmark
)

target_link_libraries(
        ccl-bench
        ccl
)

if (${PRECOMPILED_HEADER})
    target_precompile_headers(
            ccl-bench
            PUBLIC
            "pch.hpp"
    )
endif ()
