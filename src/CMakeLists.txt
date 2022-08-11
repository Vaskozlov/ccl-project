file(
        GLOB_RECURSE
        LIBCERBERUS_OBJS
        "src/*.cpp"
)

add_library(cerberus STATIC ${LIBCERBERUS_OBJS})
