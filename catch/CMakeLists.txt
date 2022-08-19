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

target_compile_definitions(
        libcerberus-catch
        PRIVATE
        "BOOST_TEST_DYN_LINK"
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
        <boost/test/unit_test.hpp>
        <cerberus/debug/debug_file.hpp>
        <boost/container/small_vector.hpp>
)

set_source_files_properties(catch/runner.cpp PRIVATE
        PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

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
