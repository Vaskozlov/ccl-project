file(
        GLOB_RECURSE
        LIBCERBERUS_OBJS
        "src/*.cpp"
)

add_library(cerberus SHARED ${LIBCERBERUS_OBJS})

target_link_libraries(
        cerberus
        fmt::fmt
)
