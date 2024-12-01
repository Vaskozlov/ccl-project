include(cmake/CPM.cmake)

find_package(Threads REQUIRED)

CPMFindPackage(
        NAME fmt
        GIT_TAG master
        GITHUB_REPOSITORY fmtlib/fmt
)

CPMFindPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        GIT_TAG devel
)

CPMFindPackage(
        NAME unordered_dense
        GITHUB_REPOSITORY martinus/unordered_dense
        GIT_TAG main
)

CPMFindPackage(
        NAME cxxopts
        GITHUB_REPOSITORY jarro2783/cxxopts
        GIT_TAG master
        OPTIONS
        "CXXOPTS_BUILD_EXAMPLES Off"
        "CXXOPTS_BUILD_TESTS Off"
)

CPMAddPackage(
        NAME isl
        GITHUB_REPOSITORY Vaskozlov/ISL
        GIT_TAG main
)

if (CCL_INCLUDE_BENCHMARK)
    CPMAddPackage(
            NAME benchmark
            GITHUB_REPOSITORY google/benchmark
            GIT_TAG main
            OPTIONS
            "BENCHMARK_ENABLE_TESTING OFF"
            "BENCHMARK_USE_LIBCXX ${CCL_LIBCPP}"
            "BENCHMARK_DOWNLOAD_DEPENDENCIES ON"
    )

    if (benchmark_ADDED)
        # patch benchmark target
        set_target_properties(benchmark PROPERTIES CXX_STANDARD 23)
    endif ()
endif ()