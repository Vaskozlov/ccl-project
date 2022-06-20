file(
        GLOB_RECURSE
        LIBCERBERUS_CATCH_OBJS
        "catch/libcerberus/*.cpp"
)

add_executable(
        libcerberus_catch
        ${LIBCERBERUS_CATCH_OBJS}
)

target_link_libraries(
        libcerberus_catch
        fmt::fmt
)

add_test(LIBCERBERUS_TEST libcerberus_catch)
