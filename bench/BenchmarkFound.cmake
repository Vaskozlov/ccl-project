file(
        GLOB_RECURSE
        CCL_BENCH_OBJECTS
        "bench/*.cpp"
)

add_executable(
        ccl-bench
        ${CCL_BENCH_OBJECTS}
)

isl_set_project_warnings(
        ccl-bench
        ${CCL_STRICT_COMPILATION}
        ""
        ""
        ""
        ""
        ${CCL_HARDENING}
        ${CCL_LIBCPP}
        OFF
)

add_dependencies(ccl-bench fmt::fmt-header-only)
add_dependencies(ccl-bench benchmark::benchmark)

target_link_libraries(
        ccl-bench
        benchmark::benchmark
)

target_link_libraries(
        ccl-bench
        ccl
)

target_link_libraries(
        ccl-bench
        fmt::fmt-header-only
)

if (${PRECOMPILED_HEADER})
    target_precompile_headers(
            ccl-bench
            REUSE_FROM
            ccl-catch
    )
endif ()
