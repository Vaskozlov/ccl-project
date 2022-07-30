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
            ${filename}-${dir_name}
            ${file}
    )

    if (PRECOMPILED_HEADERS)
        target_precompile_headers(
                ${filename}-${dir_name}
                PUBLIC
                <bitset> <map>
                <string_view>
                <cmath> <utility>
                <vector> <string>
                <initializer_list>
                <algorithm> <array>
                <numeric> <fmt/format.h>
                <boost/container/small_vector.hpp>
        )
    endif()

    target_link_libraries(
            ${filename}-${dir_name}
            fmt::fmt
    )

    target_link_libraries(
            ${filename}-${dir_name}
            ${Boost_LIBRARIES}
    )

    add_test(${filename}-${dir_name}-test ${filename}-${dir_name})

    math(EXPR i "${i} + 1")
endforeach ()
