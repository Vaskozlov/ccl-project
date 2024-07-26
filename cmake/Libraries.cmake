include(cmake/CPM.cmake)

find_package(Threads REQUIRED)

CPMAddPackage(
        NAME fmt
        GIT_TAG master
        GITHUB_REPOSITORY fmtlib/fmt
)

CPMAddPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        GIT_TAG devel
)

CPMAddPackage(
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

CPMAddPackage(
        NAME hpx
        GITHUB_REPOSITORY STEllAR-GROUP/hpx
        GIT_TAG master
        OPTIONS
        "HPX_WITH_TESTS OFF"
        "HPX_WITH_DOCUMENTATION OFF"
        "HPX_WITH_EXAMPLES OFF"
        "HPX_WITH_NETWORKING OFF"
        "HPX_WITH_DISTRIBUTED_RUNTIME OFF"
        "HPX_USE_CMAKE_CXX_STANDARD ON"
        "HPX_WITH_UNITY_BUILD OFF"
        "HPX_WITH_STATIC_LINKING ON"
        "HPX_WITH_CXX_STANDARD 23"
        "HPX_WITH_MODULES_AS_STATIC_LIBRARIES OFF"
)

include(${isl_SOURCE_DIR}/cmake/CompilerOptions.cmake)
include(${isl_SOURCE_DIR}/cmake/InterproceduralOptimization.cmake)
include(${isl_SOURCE_DIR}/cmake/ProjectOptions.cmake)

include_directories(${isl_SOURCE_DIR}/include)