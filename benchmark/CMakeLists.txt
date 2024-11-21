file(
        GLOB_RECURSE
        CCL_BENCH_OBJECTS
        "benchmark/*.cpp"
)

add_executable(
        ccl-bench
        ${CCL_BENCH_OBJECTS}
)

isl_set_target_flags(
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

if (CCL_PRECOMPILED_HEADERS)
    target_precompile_headers(
            ccl-bench
            REUSE_FROM
            ccl
    )
endif ()

add_dependencies(
        ccl-bench
        ccl
        benchmark
        astlang2
        fmt::fmt-header-only
)

target_link_libraries(
        ccl-bench
        benchmark
        ccl
        astlang2
        fmt::fmt-header-only
)

target_include_directories(ccl-bench
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
