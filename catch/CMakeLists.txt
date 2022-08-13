enable_testing()

file(
        GLOB_RECURSE
        LIBCERBERUS_CATCH
        "catch/*.cpp"
)

set(i 0)
set(first_target "")

foreach (file ${LIBCERBERUS_CATCH})

    get_filename_component(filename ${file} NAME_WLE)
    get_filename_component(file_dir "${file}" PATH)
    get_filename_component(dir_name "${file_dir}" NAME)

    add_executable(
            ${filename}-${dir_name}
            ${file}
    )

    if ((i EQUAL 0) AND (PRECOMPILED_HEADERS))
        set(first_target ${filename}-${dir_name})

        target_precompile_headers(
                ${filename}-${dir_name}
                PUBLIC
                <atomic>
                <set> <thread>
                <deque> <list>
                <bitset> <map>
                <string_view>
                <cmath> <utility>
                <vector> <string>
                <initializer_list>
                <algorithm> <array>
                <numeric> <fmt/format.h>
                <boost/container/small_vector.hpp>
        )
    endif ()

    if ((i GREATER 0) AND PRECOMPILED_HEADERS)
        target_precompile_headers(
                ${filename}-${dir_name}
                REUSE_FROM
                ${first_target}
        )

    endif ()

    target_link_libraries(
            ${filename}-${dir_name}
            fmt::fmt
    )

    target_link_libraries(
            ${filename}-${dir_name}
            ${Boost_LIBRARIES}
    )

    target_link_libraries(
            ${filename}-${dir_name}
            cerberus
    )

    add_test(${filename}-${dir_name}-test ${filename}-${dir_name})

    math(EXPR i "${i} + 1")
endforeach ()
