file(
        GLOB_RECURSE
        LIBCERBERUS_CATCH
        "catch/libcerberus/*.cpp"
)

foreach (file ${LIBCERBERUS_CATCH})

    get_filename_component(filename ${file} NAME_WLE)
    add_executable(
            ${filename}
            ${file}
    )

    target_link_libraries(
            ${filename}
            fmt::fmt
    )

    target_link_libraries(
            ${filename}
            ${Boost_LIBRARIES}
    )


    add_test(${filename}-test ${filename})
endforeach ()

