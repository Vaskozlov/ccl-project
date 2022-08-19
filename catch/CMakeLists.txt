enable_testing()

file(
        GLOB_RECURSE
        LIBCERBERUS_CATCH
        "catch/*.cpp"
)

add_executable(
        libcerberus-catch
        ${LIBCERBERUS_CATCH}
)

target_precompile_headers(
        libcerberus-catch
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

target_link_libraries(
        libcerberus-catch
        fmt::fmt
)

target_link_libraries(
        libcerberus-catch
        ${Boost_LIBRARIES}
)

target_link_libraries(
        libcerberus-catch
        cerberus
)

add_test(libcerberus-catch-test libcerberus-catch)
