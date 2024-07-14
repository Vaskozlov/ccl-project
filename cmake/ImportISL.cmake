FetchContent_Declare(
        isl
        GIT_REPOSITORY https://github.com/Vaskozlov/ISL.git
        GIT_TAG origin/main
)

FetchContent_MakeAvailable(isl)

include(${isl_SOURCE_DIR}/cmake/CompilerOptions.cmake)
include(${isl_SOURCE_DIR}/cmake/InterproceduralOptimization.cmake)
include(${isl_SOURCE_DIR}/cmake/ProjectOptions.cmake)

include_directories(${isl_SOURCE_DIR}/include)
