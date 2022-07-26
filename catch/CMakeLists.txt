enable_testing()

file(
        GLOB_RECURSE
        LIBCERBERUS_CATCH
        "catch/*.cpp"
)

set(i 0)

foreach (file ${LIBCERBERUS_CATCH})

    get_filename_component(filename ${file} NAME_WLE)
    get_filename_component(file_dir "${file}" PATH)
    get_filename_component(dir_name "${file_dir}" NAME)

    add_executable(
            ${dir_name}-${filename}
            ${file}
    )

    target_link_libraries(
            ${dir_name}-${filename}
            fmt::fmt
    )

    target_link_libraries(
            ${dir_name}-${filename}
            ${Boost_LIBRARIES}
    )

    add_test(${dir_name}-${filename}-test ${dir_name}-${filename})

    math(EXPR i "${i} + 1")
endforeach ()
