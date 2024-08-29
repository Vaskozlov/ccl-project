file(
        GLOB_RECURSE
        CCL_BENCH_OBJECTS
        "benchmark/*.cpp"
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

add_dependencies(
        ccl-bench
        ccl
        benchmark
        astlang
        fmt::fmt-header-only
)

target_link_libraries(
        ccl-bench
        benchmark
        ccl
        astlang
        fmt::fmt-header-only
)

target_include_directories(ccl-bench
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
