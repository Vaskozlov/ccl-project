include(cmake/CPM.cmake)

find_package(Threads REQUIRED)

CPMFindPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 577fd3be883accf8629423ed77fcca8a24bccee2
)

CPMFindPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        GIT_TAG 914aeecfe23b1e16af6ea675a4fb5dbd5a5b8d0a
)

CPMAddPackage(
        NAME unordered_dense
        GITHUB_REPOSITORY martinus/unordered_dense
        GIT_TAG 73f3cbb237e84d483afafc743f1f14ec53e12314
)

CPMFindPackage(
        NAME cxxopts
        GITHUB_REPOSITORY jarro2783/cxxopts
        GIT_TAG 573ec27d7224a8a0866c1889737d355b8e4155d7
        OPTIONS
        "CXXOPTS_BUILD_EXAMPLES Off"
        "CXXOPTS_BUILD_TESTS Off"
)

CPMAddPackage(
        NAME isl
        GITHUB_REPOSITORY Vaskozlov/ISL
        GIT_TAG d61f259ff4ff69482296dd1d74bd17d6e5da7bf8
)

if (CCL_INCLUDE_BENCHMARK)
    CPMAddPackage(
            NAME benchmark
            GITHUB_REPOSITORY google/benchmark
            GIT_TAG ff5c94d860dcffd4f4159edf79d805bcfaca3cb8
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