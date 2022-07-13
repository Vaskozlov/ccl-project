enable_testing()

file(
        GLOB_RECURSE
        LIBCERBERUS_CATCH_OBJS
        "catch/*.cpp"
)

add_executable(
        libcerberus_catch
        ${LIBCERBERUS_CATCH_OBJS}
)

target_precompile_headers(
        libcerberus_catch
        PUBLIC
        <bitset>
        <cmath> <utility>
        <vector> <string>
        <algorithm> <array>
        <fmt/format.h>
        <boost/container/small_vector.hpp>
)

target_link_libraries(
        libcerberus_catch
        fmt::fmt
)

target_link_libraries(
        libcerberus_catch
        ${Boost_LIBRARIES}
)

add_test(LIBCERBERUS_TEST libcerberus_catch)
