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
        NAME unordered_dense
        GITHUB_REPOSITORY martinus/unordered_dense
        GIT_TAG main
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

include(${isl_SOURCE_DIR}/cmake/CompilerOptions.cmake)
include(${isl_SOURCE_DIR}/cmake/InterproceduralOptimization.cmake)
include(${isl_SOURCE_DIR}/cmake/ProjectOptions.cmake)

include_directories(${isl_SOURCE_DIR}/include)
