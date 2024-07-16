FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
        EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(fmt)
