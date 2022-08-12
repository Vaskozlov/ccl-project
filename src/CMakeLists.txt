file(
        GLOB_RECURSE
        LIBCERBERUS_OBJS
        "src/*.cpp"
)

if (CREATE_STATIC_LIBRARY)
    add_library(cerberus STATIC ${LIBCERBERUS_OBJS})
else ()
    add_library(cerberus SHARED ${LIBCERBERUS_OBJS})
endif ()

target_link_libraries(
        cerberus
        fmt::fmt
)
