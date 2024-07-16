FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG master
        EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(cxxopts)
